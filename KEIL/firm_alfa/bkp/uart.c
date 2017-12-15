/**
  ******************************************************************************
  * @file		delay.c
  * @date		10 January 2016
  ******************************************************************************
  */
	
#include 	"uart.h"
#include	"stm32f10x_rcc.h"			//biblioteca para controle de reset e clock
#include	"stm32f10x_gpio.h"		//biblioteca para controle de gpio
#include	"stm32f10x_tim.h"			//biblioteca para acesso ao timer


void USART2_Init()
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: USART2 initialization
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
	// Step 2: GPIO initialization for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Tx pin initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART2_PutChar(char c)
{
	// Wait until transmit data register is empty
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	// Send a char using USART2
	USART_SendData(USART2, c);
}

void USART2_PutString(char *s)
{
	// Send a string
	while (*s)
	{
		USART2_PutChar(*s++);
	}
}

uint16_t USART2_GetChar()
{
	// Wait until data is received
	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	// Read received char
	return USART_ReceiveData(USART2);
}


/********************************* END OF FILE ********************************/
/******************************************************************************/
