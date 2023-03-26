#include "led.h"

#define GPIOA_EN				(1U << 0)
#define LED_PIN					(1U << 5)

void LED_Init(void)
{
	/*
	 * Enable clock access to led port
	 */
	RCC -> AHB1ENR |= GPIOA_EN;

	/*
	 * Set LED Pin as output pin
	 */
	GPIOA -> MODER |= (1U << 10);
	GPIOA -> MODER &= ~(1U << 11);
}

void LED_On(void)
{
	/*
	 * Set LED pin High (PA5)
	 */
	GPIOA -> ODR |= LED_PIN;

}

void LED_Off(void)
{
	/*
	 * Set LED pin Low (PA5)
	 */
	GPIOA -> ODR &= ~LED_PIN;
}
