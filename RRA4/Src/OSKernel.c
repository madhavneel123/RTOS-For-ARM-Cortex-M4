#include "OSKernel.h"

#define NUM_OF_THREADS				3
#define STACKSIZE					100			//100 32-bit values or 400 Bytes

struct ThreadControlBlock
{
	int32_t *StackPtr;
	struct ThreadControlBlock *NextPtr;
};

typedef struct ThreadControlBlock TcbType;

TcbType tcb[NUM_OF_THREADS];
TcbType *CurrentPtr;

/*Each Thread will have stack size of 100 i.e 400 bytes*/
int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void OSKernelStackInit(int i)
{
	tcb[i].StackPtr = &TCB_STACK[i][STACKSIZE - 16];	//Stack Pointer
	/*
	 * Set Bit 21 (T-bit) in PSR to 1 to operate in Thumb Mode
	 */
	TCB_STACK[i][STACKSIZE - 1] = (1U << 21);		//xPSR Register

	/*
	 * Dummy Stack Content - Debugging Purpose Only
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
