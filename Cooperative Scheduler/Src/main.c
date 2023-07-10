#include <stdio.h>
#include "OSKernel.h"

#define QUANTA			10  		/*10msec quanta value*/

typedef uint32_t TaskProfiler;

TaskProfiler TaskProfiler0,TaskProfiler1,TaskProfiler2;



void Task0(void)
{
	while(1)
	{
		TaskProfiler0++;
		OSThreadYield();
	}
}

void Task1(void)
{
	while(1)
	{
		TaskProfiler1++;
	}
}

void Task2(void)
{
	while(1)
	{
		TaskProfiler2++;
	}
}

int main(void)
{
	/*Initialize Kernel*/
	OSKernelInit();

	/*Add Threads*/
	OSKernelAddThreads(&Task0, &Task1, &Task2);

	/*Set Round Robin Time Quanta*/
	OSKernelLaunch(QUANTA);

}
