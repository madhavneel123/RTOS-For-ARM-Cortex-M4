#include "timebase.h"
#include "stm32f4xx.h"


#define MAX_DELAY				0xFFFFFFFF
volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;
volatile uint32_t tick_freq = 1;

void Delay(uint32_t delay)
{
	uint32_t tickstart = GetTick();
	uint32_t wait = delay;

	if(wait < MAX_DELAY)
	{
		wait += (uint32_t) (tick_freq);
	}

	while((GetTick() - tickstart) < wait);
}
void TickIncrement(void)
{
	g_curr_tick += tick_freq;
}

uint32_t GetTick(void)
{
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();
}

void TimeBase_Init(void)
{
	/*
	 * Reload the timer with number of cycles per second
	 */
	SysTick -> LOAD = 15999999;

	/*
	 * Clear SysTick current value register
	 */
	SysTick -> VAL = 0;

	/*
	 *Select Internal Clock source
	 */
	SysTick -> CTRL |= (1U << 2);

	/*
	 * Enable Interrupt
	 */
	SysTick -> CTRL |= (1U << 1);

	/*
	 * Enable SysTick
	 */
	SysTick -> CTRL |= (1U << 0);

	__enable_irq();
}

void SysTick_Handler(void)
{
	TickIncrement();
}
