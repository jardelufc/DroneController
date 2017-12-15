/**
  ******************************************************************************
  * @file		delay.c
  * @author	Yohanes Erwin Setiawan
  * @date		10 January 2016
  ******************************************************************************
  */
	
#include "../inc/delay.h"

// For store tick counts in us
static __IO uint32_t usTicks;
uint32_t counter;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	//if (usTicks != 0)
	//{
	//	usTicks--;
	//}
	usTicks++;
}

void DelayInit()
{
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000);
}

void reset_time()
{
	usTicks = 0;
}

//void delay_us(uint32_t us)
//{
//	// Reload us value
//	usTicks = us;
//	// Wait until usTick reach zero/
//	while (usTicks);
//}

void delay_ms(uint32_t ms)
{
	// Wait until ms reach zero
	counter = usTicks + ms;
	
	while (usTicks<counter);
}

uint32_t millis()
{
	return usTicks;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
