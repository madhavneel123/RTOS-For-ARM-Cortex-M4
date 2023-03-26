#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#include "stm32f4xx.h"
uint32_t GetTick(void);
void Delay(uint32_t delay);
void TimeBase_Init(void);
void TickIncrement(void);

#endif /* TIMEBASE_H_ */
