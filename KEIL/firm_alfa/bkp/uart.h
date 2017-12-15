/**
  ******************************************************************************
  * @file		uart.h
  * @date		10 January 2016
  ******************************************************************************
  */
	
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
 extern "C" {
#endif

#include	"stm32f10x.h"

void USART2_Init(void);
void USART2_PutChar(char c);
void USART2_PutString(char *s);
uint16_t USART2_GetChar(void);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
