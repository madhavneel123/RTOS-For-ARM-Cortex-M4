#include "led.h"

int main(void)
{
	LED_Init();

	while(1)
	{
		LED_On();
		for(int i = 0 ; i < 9000 ; i++){}
		LED_Off();
	}

}
