#include "led.h"
#include "uart.h"
#include <stdio.h>
#include "timebase.h"

void run(void);
void stop(void);
void open(void);
void close(void);

void run(void)
{
	UartWrite('R');

}

void stop(void)
{
	UartWrite('S');
}

void open(void)
{
	UartWrite('O');
}

void close(void)
{
	UartWrite('C');
}
void Mmain(void)
{
	while(1)
	{
		run();
		Delay(1);
		stop();
		Delay(1);
	}
}

void Vmain(void)
{
	while(1)
	{
		open();
		Delay(1);
		close();
		Delay(1);
	}
}





int main(void)
{
	uint32_t flag = 0;
	LED_Init();
	UART_Tx_Init();

	printf("Hello World");

	UartWrite(5);
	TimeBase_Init();
	if(flag)
		Mmain();
	else
		Vmain();
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
