#include "led.h"
#include "uart.h"
#include <stdio.h>
#include "timebase.h"

int main(void)
{
	LED_Init();
	UART_Tx_Init();

	printf("Hello World");

	UartWrite(5);
	TimeBase_Init();
	while(1)
	{
		UartWrite(5);
		printf("Hello World");
		Delay(1);
		LED_On();
		Delay(1);
		LED_Off();
	}

}