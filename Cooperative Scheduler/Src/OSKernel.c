#include "OSKernel.h"

#define NUM_OF_THREADS				3			//Define Number of threads
#define STACKSIZE					100			//Define Stack size.
#define BASE_FREQ					16000000			//Base Frequency is 16MHz

#define CTRL_ENABLE				(1U << 0)
#define CTRL_TICKINT			(1U << 1)
#define CTRL_CLCSRC				(1U << 2)
#define CTRL_COUNTFLAG			(1U << 16)
#define SYSTICK_RST				0
#define INTCTRL					(*(volatile uint32_t *)0xE000ED04)
#define PENDSTSET				(1U << 26)

uint32_t MILLI_PRESCALER;

struct ThreadControlBlock
{
	int32_t *StackPtr;							//StackPointer
	struct ThreadControlBlock *NextPtr;			//Linked List
};

typedef struct ThreadControlBlock TcbType;

TcbType tcb[NUM_OF_THREADS];					//Array of Thread Control Block
TcbType *CurrentPtr;							//Declare Current Pointer

/*Each Thread will have stack size of 100 i.e 400 bytes*/
int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void OSKernelStackInit(int i)
{
	tcb[i].StackPtr = &TCB_STACK[i][STACKSIZE - 16];	//R13 Stack Pointer initialization
	/*
	 * Set Bit 21 (T-bit) in PSR to 1 to operate in Thumb Mode
	 */
	TCB_STACK[i][STACKSIZE - 1] = (1U << 24);		//xPSR Register

	/*
	 * Dummy Stack Content - Debugging Purpose Only. It denotes initialization of stack.
	 * We skipped R13 (SP).
	 */
	TCB_STACK[i][STACKSIZE - 3] = 0xAAAAAAAA;		//R14 - LR
	TCB_STACK[i][STACKSIZE - 4] = 0xAAAAAAAA;		//R12
	TCB_STACK[i][STACKSIZE - 5] = 0xAAAAAAAA;		//R3
	TCB_STACK[i][STACKSIZE - 6] = 0xAAAAAAAA;		//R2
	TCB_STACK[i][STACKSIZE - 7] = 0xAAAAAAAA;		//R1
	TCB_STACK[i][STACKSIZE - 8] = 0xAAAAAAAA;		//R0

	TCB_STACK[i][STACKSIZE - 9] = 0xAAAAAAAA;		//R11
	TCB_STACK[i][STACKSIZE - 10] = 0xAAAAAAAA;		//R10
	TCB_STACK[i][STACKSIZE - 11] = 0xAAAAAAAA;		//R9
	TCB_STACK[i][STACKSIZE - 12] = 0xAAAAAAAA;		//R8
	TCB_STACK[i][STACKSIZE - 13] = 0xAAAAAAAA;		//R7
	TCB_STACK[i][STACKSIZE - 14] = 0xAAAAAAAA;		//R6
	TCB_STACK[i][STACKSIZE - 15] = 0xAAAAAAAA;		//R5
	TCB_STACK[i][STACKSIZE - 16] = 0xAAAAAAAA;		//R4

}


uint8_t OSKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void))
{
	/*
	 * Disable Global Interrupts
	 */
	__disable_irq();
	tcb[0].NextPtr = &tcb[1];
	tcb[1].NextPtr = &tcb[2];
	tcb[2].NextPtr = &tcb[0];

	/*Initalize stack for thread 0*/
	OSKernelStackInit(0);

	/* Initialize Program Counter*/
	TCB_STACK[0][STACKSIZE - 2] = (uint32_t)(task0);


	/*Initalize stack for thread 1*/
	OSKernelStackInit(1);

	/* Initialize Program Counter*/
	TCB_STACK[1][STACKSIZE - 2] = (uint32_t)(task1);


	/*Initalize stack for thread 2*/
	OSKernelStackInit(2);

	/* Initialize Program Counter*/
	TCB_STACK[2][STACKSIZE - 2] = (uint32_t)(task2);

	/*Start from thread 0*/
	CurrentPtr = &tcb[0];

	/*Enable Global Interrut*/
	__enable_irq();

	return 1;

}

void OSKernelInit(void)
{
	 MILLI_PRESCALER = (BASE_FREQ)/1000;
}

void OSKernelLaunch(uint32_t quanta)
{
	/* Clear SYSTICK Timer */
	SysTick ->CTRL = SYSTICK_RST;

	/*Load Qunata*/
	SysTick -> LOAD = (quanta * MILLI_PRESCALER) - 1;

	/*Clear current value register*/
	SysTick -> VAL = 0;

	/*Set Systick to low priority. So that task can have more priority.*/
	NVIC_SetPriority(SysTick_IRQn,15);

	/*Enable Systick. Select Internal Clock */
	SysTick -> CTRL = CTRL_CLCSRC | CTRL_ENABLE;

	/*Enable Systick Interrupt*/
	SysTick -> CTRL |= CTRL_TICKINT;

	/*Launch Scheduler*/
	OSSchedulerLaunch();
}


/*
 * We don't want stack to be changed during the execution to ISR
 * When exception occurs  R0 R1 R2 R3 R12 LR PC PSR are saved automatically onto stack
*/

__attribute__((naked)) void SysTick_Handler(void)
{
	/* Suspend Current Thread */


	/*Disable Global interrupts as we need operation to be atomic*/
	__asm("CPSID I");

	/* Save R4, R5, R6, R7, R8, R9, R10, R11 manually as rest of the registers are saved automatically*/
	__asm("PUSH {R4-R11}");

	/* Load address of current pointer to R0 */
	__asm("LDR R0, =CurrentPtr");

	/*Load R1 from address equals R0, i.e. R1 = CurrentPtr*/
	__asm("LDR R1, [R0]");

	/*Store Cortex M SP at address equals R1, i.e Save SP into Tcb */
	__asm("STR SP, [R1]");

	/* Choose Next Thread */

	/*Load R1 from a location 4 byte above address R1 i.e R1 = CurrentPtr -> Next*/
	__asm("LDR R1, [R1, #4]");

	/*Store R1 at address equal R0 i.e. CurrentPtr = R1*/
	__asm("STR R1, [R0]");

	/*Load Cortex M SP from address equals R1, i.e. SP = CurrentPtr -> StackPtr*/
	__asm("LDR SP, [R1]");

	/*Restore R4-R11*/
	__asm("POP {R4-R11}");

	/*Enable Global Interrupt */
	__asm("CPSIE I");

	/*Return from exception and restore R0-R1 R2 R3 R12 LR PC PSR*/
	__asm("BX LR");
}

void OSSchedulerLaunch(void)
{
	/* Load address of CurrentPtr to R0*/
	__asm("LDR R0,=CurrentPtr");

	/*Load R2 from address equals R0, R2=CurrentPtr*/
	__asm("LDR R2,[R0]");

	/*Load Cortex M4 SP from address equals R2 i.e SP = CurrentPtr ->StackPtr */
	__asm("LDR SP,[R2]");

	/*Restore R4-R11*/
	__asm("POP {R4-R11}");

	/*Restore R12*/
	__asm("POP {R12}");

	/*Restore R0-R3*/
	__asm("POP {R0-R3}");

	/*Skip LR and PSR*/
	__asm("ADD SP, SP, #4");

	/*Create a new start location by popping LR*/
	__asm("POP {LR}");

	/*Skip PSR by adding 4 to SP*/
	__asm("ADD SP,SP,#4");

	/*Enable Global Interrupt*/
	__asm("CPSIE I");

	/*Return from exception*/
	__asm("BX LR");
}

void OSThreadYield(void)
{
	/*Clear SYSTICK Current Value Register*/
	SysTick -> VAL = 0;

	/*Trigger SYSTICK*/
	INTCTRL = PENDSTSET;

}
