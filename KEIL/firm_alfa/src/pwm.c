/**
  ******************************************************************************
  * @file    pwm.c
  * @date    18-11-2017
  * @brief   Este arquivo contem funções auxiliares para controle do pwm
  ******************************************************************************
  * <h2><center>&copy; STM32F103C8 </center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include	"../inc/pwm.h"
#include	"../inc/delay.h"
#include	"../inc/controle.h"
#include	"stm32f10x_rcc.h"			//biblioteca para controle de reset e clock
#include	"stm32f10x_tim.h"			//biblioteca para acesso ao timer
#include	"stm32f10x_gpio.h"		//biblioteca para controle de gpio

/* Variaveis -----------------------------------------------------------------*/
float	vel_tmp = PWM_min, R[3];
float	error, lastProcess = 0, P, I, D;
uint8_t state = 0;
uint16_t	MotorA,	MotorB, MotorC, MotorD;		//potencia do bruta do motor

uint16_t man_a, man_b, man_c, man_d;
/* Funcoes Defines ---------------------------------------------------------*/
			
/* Funcoes -----------------------------------------------------------------*/
// For store tick counts in us
void PWM_Init()
{
	//Estruturas
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//configura timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitStruct.TIM_Prescaler	= 71;
	TIM_TimeBaseInitStruct.TIM_Period		= pwm_period;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIMx, ENABLE);				
	
	//configura pwm
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIMx, &TIM_OCInitStruct);	//CH 1 | PB06
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);	//CH 2 | PB07
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);	//CH 3 | PB08
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);	//CH 4 | PB09
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	
	//configura portas de saida
	//CH 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void PWM_Cali()
{
	TIM_SetCompare1(TIMx, PWM_max);
	TIM_SetCompare2(TIMx, PWM_max);
	TIM_SetCompare3(TIMx, PWM_max);
	TIM_SetCompare4(TIMx, PWM_max);
	delay_ms(5000);
	
	TIM_SetCompare1(TIMx, PWM_min);
	TIM_SetCompare2(TIMx, PWM_min);
	TIM_SetCompare3(TIMx, PWM_min);
	TIM_SetCompare4(TIMx, PWM_min);
	delay_ms(1000);
}

void PWM_pwrcrtl(uint8_t tmp)							//tmp[0,255]
{
	if (tmp == 3){
		if (state == 0)	vel_tmp = 1350;
		else 						vel_tmp = PWM_min;
		state=~state;
	} else if ((tmp == 1)&&(vel_tmp > 1000)){
		vel_tmp -= PWM_step;
	} else if ((tmp == 2)&&(vel_tmp < 2000)){
		vel_tmp += PWM_step;
	}
}
void PWM_dircrtl(uint8_t dir, uint8_t pwr)
{
	if(state !=0){
		if	(dir == 0){
			man_a = man_d = -pwr*20;
			man_b = man_c = pwr*20;
		}
		else if	(dir == 1){
			man_c = pwr*20;
			man_a = -pwr*20;
			man_b = man_d = 0;
		}
		else if	(dir == 2){
			man_c = man_d = pwr*20;
			man_a = man_b = -pwr*20;
		}
		else if	(dir == 3){
			man_d = pwr*20;
			man_b = -pwr*20;
			man_a = man_c = 0;
		}
		else if	(dir == 4){
			man_a = man_d = pwr*20;
			man_b = man_c = -pwr*20;
		}
		else if	(dir == 5){
			man_a = pwr*20;
			man_c = -pwr*20;
			man_b = man_d = 0;
		}
		else if	(dir == 6){
			man_c = man_d = -pwr*20;
			man_a = man_b = pwr*20;
		}
		else if	(dir == 7){
			man_b = pwr*20;
			man_d = -pwr*20;
			man_a = man_c = 0;
		}
	}
		//else if	(dir == 2)	man_y = intencidade(pwr);
		//else if	(dir == 4)	man_x = -intencidade(pwr);
		//else if	(dir == 6)	man_y = -intencidade(pwr);
}

/*
void PWM_pid(uint8_t angle,uint8_t set_point,uint16_t dt)
{
	error = set_point - angle;
	uint16_t deltaTime = (dt - lastProcess) / 1000.0;
	lastProcess = dt;
	
	P = error * Kp;
	
	I = I + (error * Ki) * deltaTime;
	I = I;
	
	D = (lastError - angle) * Kd / deltaTime;
	lastError = error;
	
	//Soma tudo e Converte graus em PWM
	return vel_pid = map((P + I + D), 90, 0, PWM_max, PWM_min);
}
*/

void PWM_update()
{
	MotorA = vel_tmp + man_a;																				//CH 1 | PB06 
	MotorB = vel_tmp + man_b;																				//CH 2 | PB07
	MotorC = vel_tmp + man_c;																				//CH 3 | PB08
	MotorD = vel_tmp + man_d;																				//CH 4 | PB09
	
	TIM_SetCompare1(TIMx, MotorA);																	//Atualiza pwm do CH1
	TIM_SetCompare2(TIMx, MotorB);																	//Atualiza pwm do CH2
	TIM_SetCompare3(TIMx, MotorC);																	//Atualiza pwm do CH3
	TIM_SetCompare4(TIMx, MotorD);																	//Atualiza pwm do CH4
}


/********************************* END OF FILE ********************************/
/******************************************************************************/
