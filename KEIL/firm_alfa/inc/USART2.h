#ifndef __USART2_H
#define __USART2_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
	 
	 void USART2_Putch_Data(uint16_t data);
	 void USART2_Putch(unsigned char ch);
	 void USART2_Print(char s[]);
	 void USART2_Print_Int(int number);
	 void USART2_Print_Float(double number);
	 uint8_t USART2_Get_Int(void);
	 void USART2_Init(void);

#ifdef __cplusplus
}
#endif

#endif
