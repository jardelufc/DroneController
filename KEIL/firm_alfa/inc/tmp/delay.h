
#ifndef __DELAY_H
#define __DELAY_H


#include <stm32f10x.h>


// Function prototypes
void DelayInit(void);
void delay_ms(uint32_t ms);
void delaysleep_ms(uint32_t ms);
void reset_millis(void);
uint32_t millis(void);

#endif // __DELAY_H
