
  <h1 align="center">RTOS for ARM Cortex M4</h1>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#introduction-to-operating-system">Introduction to Operating System</a>
      <ul>
        <li><a href="#the-stack">The Stack</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
# About The Project






<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Basics of ARM Cortex M4-->
# Basics of ARM Cortex M4
The cortex M4 processor is based on the ARMv7E-M architecture. 

## Programmers Model
M4 processor has two opeartion states and two modes. In addition processor hava privileged and unprivileged access levels. Privileged access level can access all resources in the processor. In some text it is also referred as User mode and kernel mode.

### Operation States

1. Debug State - When processor is halted it enters debug state. It can be halted if it hits a breakpoint
2. Thumb State - If processor is in running mode it is in thumb state. There is no ARM state in M4 as it was in ARM7TDMI.

### Operation Modes

1. Handler Mode - When executing ISR. In handler mode processor is always on privileged access level
2. Thread Mode - Execution in norma; application code. Processor is either in privileged access level or unprivileged access level. This is controlled by special register called CONTROL

Software can switch from privileged thread mode to unprivileged thread mode but vice versa is not possible. If it needs to be done then processor has to use exception mechanism to handle the switch.

### Registers
ARM Cortex M4 architecture has 16 register namely R0-R12, R13 (SP), R14(LR), R15(PC). 
1. R0-R7 are called low registers. Many 16 bit instructions can only access low registers
R8-R12 are high registers. It can be used with 32 bit instructions and few 16bit instructions like MOV.
Initial value of R0-R12 is undefined

2. R13 (SP) it is used to access stack memory using PUSH and POP operation. There are two Stack pointers - Main Stack Pointer (MSP) and Process Stack Pointer(PSP). MSP is default stack pointer. PSP can be used only the thread mode. Selection of stack pointer is done using CONTROL register.
3. R14 (LR) It is used to hold return address when calling a function or subroutine. During return of subroutine LR value is copied to PC.
4. R15 (PC) Writing to PC results in branch operation. LSB of PC is always set to 0 to denote thumb state.
5. xPSR (Program Status Register)
6. PRIMASK It is 1 bit wide interrupt mask register. When set it blocks all exceptions aprt from NMI and hard fault exceptions. It raises current exception priority to 0.
7. FAULTMASK It is similar to PRIMASK but it also blocks hard fault exception. It blocks current exception priority to -1.
8. BASEPRI It allows more flexible interrupt masking. It can be of 3-7 bits length (depends of vendor). If it is set to 0 then BASEPRI is disabled. If it is set to a non zero value it masks all interrupt with same or lower priority level.
9. CONTROL It is used for selection of MSP or PSP, Privileged or unprivileged access levels in thread mode, Floating point unit used or unused

MSR or MRS instrutions are used to change values of special register

## Exception and Interrupts
Exceptions are events that causes changes to program flow. When exception happens processor suspends the current executing task and executes a part of program called exception handler. After execution of exception handler is completed the processor resumes normal program execution. 

### Reset Sequence
There are three types of RESET in M4 - 
1. Power on Reset - Reset everyting in the microcontroller.This includes the processor and its debug support components and peripherals
2. System Reset - Reset just the processor and peripherals but not debug support components of the processor
3. Processor Reset - Reset processor only.

After reset and before the processor starts executing the program the M4 processor read first two words from the memory. The begining of memory space contains vector table and first word is MSP. Second word is staring address of reset hanlder. After these two values are read by the processor the processor sets up the MSP and the PC with these values. Set up of MSP is necessary as NMI or hard faults can occur shrtly after reset and MSP has to PUSH status of processor to the stack before the exception handling. The stack operation in M4 are based on full decending stack.

### NVIC
It is one of the periheral of ARM Cortex M4. It is used to control 240 interrupts. Using NVIC you can 

<!-- Introduction to Operating System -->
# Introduction to Operating System
Operating system is a resource manager. It controls the hardware peripherals and schedule task. Real time operating system is also an operating system with time costaints. For real time operating system not only execution of task is important but also time frame in which execution is completed. RTOS meets the deadline. RTOS performance metrics are of two types-

1. Deadlines - Completion of task in specified amount of time
2. Reliablity - Running of task in predicted way.

## The Stack
Stack is Last in First out temporary storage. To create a stack block of ROM is allocated. Stack always operate on 32bit data. R13 is stack pointer which always point to top of the stack. Stack grows downward the memory as we push data onto it.
Stack Operations
1. PUSH - Saves values into the stack memory pointer by stack pointer (R13). First data is pushed into the stack and then SP is decremented. 
2. POP - Retrive values from the stack memory pointer by stack pointer (R13). First SP is incremented and then data is pulled.

## Inside Microcontroller
In ARM Cortex M4 architecture we have 
1. 12 general purpose registers (R0 - R12)
2. R13 - Stack Pointer - There are two SP. Main Stack Pointer (MSP) and Process Stack Pointer (PSP)
3. R14 - Link Register - Used to store interm location of functions
4. R15 - Program Counter - Both readable and writetable. Read from this register gives us address of current instruction + 4. Write into this register makes a branch instruction. 

Special Register
1. PSR
2. PRIMASK
3. FAULTMASK
4. BASEPRI
5. CONTROL

### Stack Pointer
1. MSP - It is often used in OS kernel. If you run an operating system use MSP to manipulate and access the kernel. You also use MSP in interrupt service routine. MSP is always used in handler mode. By default we use MSP
2. PSP - It is used in application task. 

Access Instruction
1. MSR
2. MRS

### Exclusive Access Instructions
1. Semaphore
2. Mutex

### SYSTICK
System Tick or SYSTICK is a core device for all ARM Cortex M4 controllers which is used to create periodic timers. It is 24 bit down counter that runs at bus clock frequency. SYSTICK registers
1. SYSTICK Control and Status Registers
2. SYSTICK Reload Value Registers
3. SYSTICK Current Value Registers
4. SYSTICK Priority Registers

Steps for SYSTICK configuration
1. Disable SYSTICK
2. Set Period
3. Clear Initial value
4. Set SYSTICK priority
5. Enable SYSTICK
6. Set Clock Source
7. Enable Interrupt

### Boot Sequence of ARM Cortex M4
When we press reset button boot sequnce begins. We can generate software reset as well. Set all registers to their reset values. Then process detects boot mode. Now fetch MSP from address 0x00000000. Now fetch PC from 0x00000004
1. The first 32 bit value in the first address is the value to initialize the MSP register.
2. The next 32 bit value is the value to initialize PC. It consists of address of Reset Handler.
3. Reset handler is a subroutine written in assembly and it constisn entry point of the program. In reset handler there is call to main().  
4. In startup.s file we have reset handler. It is a subroutine. "LDR R0, =__main" tells the processor to start from main function.
5. When processor is booted it reads two pins boot 0 and boot 1 to determine boot mode. It tells the processor where to boot from - Flash memory, SRAM or any other memory location. 
6. After reading boot 0 and boot 1 pins processor then fetches the MSP.
7. LSB of PC should always be 1 in ARM Cortex M4 controllers because of thumb mode.

### Threads
Threads consists of set of registes, stack and program to execute. Suppose we have four task to execute. Then each task is a thread which consists of their own registers, stack and program to execute. But we have only one regsiter bank how can we handle four register bank to represent four thread. Here thread scheduler comes into picture. If we have four cores then we have enough resources to handle four threads in parallel. Threads are of three types
1. Timed Threads - It include threads which donnot run frequently. Such as thread that only runs when there is a problem. This type of threads are called Sporadic threads. Another type of timed threads are periodic threads which runs in a fixed time interval. We also have aperiodic threads that execute frequently but execution cannot be anticipated. 
2. Event triggered Threads - One type is flag thread which runs only when flag is high or low. Another one in input triggered which runs only when input is available. We also have output triggered thread which runs when output is available
3. Main Threads - They are executed when nothing is left to execute. 

#### Thread Control Block (TCB)
TCB is a Data Structure containing information private to each thread. 
Must have
1. Pointer to the stack. 
2. Pointer to the next thread
3. Variable to hold thread status
4. Variable to hold thread ID
5. Variable to hold thread period
6. Variable to hold thread burst time
7. Variable to hold thread priority

### Scheduler
Scheduler is used to schedule threads based on algorithm. Thread has three state
1. RUNNING - Currently beign executed by the CPU
2. READY - It is ready to be executed by the CPU
3. BLOCKED - It is waiting for an event

At a time only one thread is in RUNNING state. Other threads are either in READY or BLOCKED state. 

PROCESS - Execution of program instance
THREAD - Execution of task
THreads are part of process. Process has higher contex switch overhead

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#### Classification of Schedulers
1. Static vs Dynamic
2. Preemptive vs Non-Preemptive

#### Scheduling Criteria
1. Throughput - Task completed per unit time
2. Turnaround time - Time for each task to complete
3. Response time - Time from request to first response of task
4. CPU Utilization - Percentage of available CPU cycles begin used
5. Wait time - Time each task waits in ready queue

#### Scheduling Algorithm Optimization
1. Maximize Throughput
2. Minimize Turnaround time
3. Minimize Response time
4. Maximize CPU Utilization
5. Minimize Scheduling Overheads

#### Application Information
1. Number tasks
2. Resource requirements of each task
3. Release time for each task
4. Deadlines for each task

#### Platform Specific Information
1. Maximum Context Switching time
2. Maximum Interrupt Service Latency
3. Maximum Communication Delay

### First Come First Serve Scheduler
1. Task are executed on first come first serve basis
2. Non - Preemptive
3. Its Implementation is based on FIFO queue
4. Poor in Performance as average wait time is high

### Round Robin Scheduler
1. Preemptive
2. Employs time sharing, gives each thread a timeslice (quanta)
3. When timeslice runs out OS prempts the thread

#### Quanta
If qunta is very large Round Robin scheduler works similar to FCFS. If qunata is very small lots of context switching and more time is spent. It drains lots of CPU time. 

#### Weighted Round Robin Scheduler
1. Preemptive
2. Employs time sharing, gives each thread a timeslice (quanta)
3. If qunata runs out OS preempts the thread
4. Threads have unequal weights so that threads with greater weight have more time qunata.
5. Can be done using varying qunata (Multiply weight with qunata) or by varying frequncy (call important task frequntly with same qunata)

#### Context Switch
It means switching from one thread to another and it invovles saving the state oof current thread into memory, restoring the state of new thread from memory and lauching the new thread. When an exception occurs certain registers needs to be preserved into stack. Stack is memory in RAM that is used to this purpose. When processor takes an exception the processor pushes information onto the current stack. This operation is referred to as stacking. and structure of eight data words is refered as the stack frame. 
1. Exception Frame without Floating point storage - R0, R1, R2, R3, R12, LR, PC, xPSR
2. Exception Frame with Floating point storagr - R0, R1, R2, R3, R12, LR, PC, xPSR, S0-S15, FPSCR

PC value always points to next address to executed. 
R0- R7 are called low registers and R8- R12 are called high registers. 

#### Kernel
Scheduler is one of the component of kernel. Scheduler is responsible for selection of thread to run. Kernel however are responsible for scheduler, booting, inter thread communication and synchronization. 
<!-- CONTRIBUTING -->
## Contributing

 **greatly appreciated**.



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. I've included a few of my favorites to kick things off!

* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Malven's Flexbox Cheatsheet](https://flexbox.malven.co/)
* [Malven's Grid Cheatsheet](https://grid.malven.co/)
* [Img Shields](https://shields.io)
* [GitHub Pages](https://pages.github.com)
* [Font Awesome](https://fontawesome.com)
* [React Icons](https://react-icons.github.io/react-icons/search)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
