#include <stdio.h>
#include "OSKernel.h"

#define QUANTA			2  		/*10msec quanta value*/

typedef uint32_t TaskProfiler;

TaskProfiler TaskProfiler0,TaskProfiler1,TaskProfiler2,pTaskProfiler3;
int32_t Semaphore1, Semaphore2;


void Task3(void)
{
	pTaskProfiler3++;

}

void Task0(void)
{
	while(1)
	{
		TaskProfiler0++;
	}
}

void Task1(void)
{
	while(1)
	{
		//TaskProfiler1++;
		OSSemaphoreWait(&Semaphore1);

		//Print some data
		OSSemaphoreSet(&Semaphore2);

	}
}

void Task2(void)
{
	while(1)
	{
		//TaskProfiler2++;
		OSSemaphoreWait(&Semaphore2);

		//Print some data

		OSSemaphoreSet(&Semaphore1);
	}
}

int main(void)
{
	/*Initialize Semaphore*/
	OSSemphoreInit(&Semaphore1, 1);
	OSSemaphoreInit(&Semaphore2, 0);
	/*Initialize Kernel*/
	OSKernelInit();

	/*Add Threads*/
	OSKernelAddThreads(&Task0, &Task1, &Task2);

	/*Set Round Robin Time Quanta*/
	OSKernelLaunch(QUANTA);

}
