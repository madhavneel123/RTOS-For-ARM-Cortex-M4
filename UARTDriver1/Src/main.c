#include "led.h"
#include "uart.h"
#include <stdio.h>


int main(void)
{
	LED_Init();
	UART_Tx_Init();
	while(1)
	{
		printf("Hello from STM32F4 .......\n");
	}

}