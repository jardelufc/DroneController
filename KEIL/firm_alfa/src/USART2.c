/**
  ******************************************************************************
  * @file    pwm.c
  * @date    18-11-2017
  * @brief   Este arquivo contem funções auxiliares para 
  ******************************************************************************
  * <h2><center>&copy; STM32F103C8 </center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "../inc/USART2.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


/* Function definitions ------------------------------------------------------*/
void USART2_Putch_Data(uint16_t data)
{
	USART_SendData(USART2, data);

	// Wait until the end of transmision
	while( USART_GetFlagStatus( USART2, USART_FLAG_TC) == RESET){}
}

void USART2_Putch(unsigned char ch)
{
	USART_SendData( USART2, ch);

	// Wait until the end of transmision
	while( USART_GetFlagStatus( USART2, USART_FLAG_TC) == RESET){}
}


void USART2_Print(char s[])
{
    int i=0;
    
    while( i < 64)
	{
	    if( s[i] == '\0') break;
        USART2_Putch( s[i++]);
    }	
}


void USART2_Print_Int(int number)
{
	unsigned char s[5], i=1, j=0;

    if( number < 0)
    { 
    	USART2_Putch( '-');
		number = -number;
	}	
 
    while( number >= 10)
    {
	    s[i++] = number % 10;
	    number /= 10;
    }
    s[i] = number;
    j = i;
    for( i=j; i>0; i--) USART2_Putch( '0' + s[i]);
}

void USART2_Print_Float(double number)
{
    double float_side;
    int int_side;
    
    int_side = number;
    float_side = number - int_side; 
    USART2_Print_Int(int_side);
    USART2_Print(".");
    
    /* We only print 2 decimals after quota */
    if (float_side < 0) float_side *= -1;
    USART2_Print_Int(float_side * 100);
}

uint8_t USART2_Get_Int ()
{
    //assert_param(IS_USART_123_PERIPH(USARTx));

    if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) return  USART2->DR & 0xff;
		else return 0;
}

void USART2_Init(void)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
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
	
	// Step 3: Enable USART receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	// Step 4: Initialize NVIC for USART IRQ
	// Set NVIC prority group to group 4 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// Set System Timer IRQ at higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
	// Set USART2 IRQ at lower priority
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
