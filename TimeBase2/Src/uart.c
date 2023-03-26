#include "uart.h"
#include "stm32f4xx.h"
#include <stdio.h>


#define GPIOA_EN			(1U << 0)
#define UART2_EN			(1U << 17)
#define SYS_FREQ			16000000
#define APB1_CLK			SYS_FREQ
#define UART_BAUDRATE		115200
#define CR1_TE				(1U << 3)
#define CR1_UE				(1U << 13)
#define SR_TXE				(1U << 7)

static uint32_t ComputeUart_BR(uint32_t periph_clk, uint32_t baudrate);
static void UartSet_BR(uint32_t periph_clk, uint32_t baudrate);


int __io_putchar(int ch)
{
	UartWrite(ch);
	return ch;
}

void UART_Tx_Init(void)
{
	/*
	 * Enable CLock access to GPIO A
	 */
	RCC -> AHB1ENR |= GPIOA_EN;

	/*
	 * Set PA2 mode to alternate functionality mode
	 */
	GPIOA -> MODER &= ~(1U << 4);
	GPIOA -> MODER |= (1U << 5);

	/*
	 * Set Alternate function type to AF7 (UART2_TX)
	 */
	GPIOA -> AFR[0] |= (1U << 8);
	GPIOA -> AFR[0] |= (1U << 9);
	GPIOA -> AFR[0] |= (1U << 10);
	GPIOA -> AFR[0] &= ~(1U << 11);

	/*
	 * Enable clock access to UART
	 */
	RCC -> APB1ENR |= UART2_EN;

	/*
	 * Enable UART Module
	 *
	 * */
	USART2 -> CR1 |= CR1_UE;
	USART2 -> CR1 &= ~(1 << 12);
	USART2 -> CR2 &= ~ (0b11 << 12);
	/*
	 * Configure Baud Rate
	 */
	//UartSet_BR(APB1_CLK, UART_BAUDRATE);
	USART2 -> BRR = (0x113);
	/*
	 * Configure Transfer Direction
	 */
	USART2 -> CR1 |= CR1_TE;
	USART2 -> CR1 |= ( 1 << 15);
	//USART2 -> DR = 0xF5;


	//UartWrite(5);

}

void UartWrite(int ch)
{
	/*
	 * Make sure the transmit data register is empty
	 */
	while(!(USART2 -> SR & SR_TXE));
	/*
	 * Write to transmit data register
	 */
	USART2 -> DR = (ch & 0xFF);


}

static void UartSet_BR(uint32_t periph_clk, uint32_t baudrate)
{
	USART2 -> BRR = ComputeUart_BR(periph_clk, baudrate);
}

static uint32_t ComputeUart_BR(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U)/baudrate));
}
