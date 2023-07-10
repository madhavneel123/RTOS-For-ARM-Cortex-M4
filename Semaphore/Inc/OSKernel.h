
#ifndef OSKERNEL_H_
#define OSKERNEL_H_

#include <stdint.h>
#include "stm32f4xx.h"

void OSKernelLaunch(uint32_t quanta);
uint8_t OSKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void));
void OSKernelStackInit(int i);
void OSSchedulerLaunch(void);
void OSKernelInit(void);
void OSThreadYield(void);
void OSCchedulerRoundRobin(void);

void OSSemaphoreInit(int32_t *semaphore, int32_t value);
void OSSemaphoreSet(int32_t *semaphore);
void OSSemaphoreWait(int32_t *semaphore);

void Task3(void);


#endif /* OSKERNEL_H_ */
