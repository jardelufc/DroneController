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

#include	"../inc/controle.h"

/* Variaveis -----------------------------------------------------------------*/
float dt, y, S;
float K_0, K_1;

/* Funcoes -------------------------------------------------------------------*/
uint16_t map(uint16_t var, uint16_t in_max, uint16_t in_min, uint16_t out_max, uint16_t out_min)
{
	return (var -  in_min)*(out_max-out_min)/(in_max - in_min) + out_min;
}

void kalman_InitStruct(kalmanStruct* kalman)
{
	kalman->Q_angle	=	0.001;
	kalman->Q_gyro	=	0.003;
	kalman->R_angle	=	0.03;
	kalman->angle	= 0;
	kalman->bias	= 0;
	kalman->P_00 = 0;
	kalman->P_01 = 0;
	kalman->P_10 = 0;
	kalman->P_11 = 0;
}

float kalman_filter(kalmanStruct* kalman, float newAngle, float newRate, uint32_t looptime)
{
	dt = (float)looptime/1000;
	kalman->angle += dt * (newRate - kalman->bias);
	kalman->P_00 +=  - dt * (kalman->P_10 + kalman->P_01) + kalman->Q_angle * dt;
	kalman->P_01 +=  - dt * kalman->P_11;
	kalman->P_10 +=  - dt * kalman->P_11;
	kalman->P_11 +=  + kalman->Q_gyro * dt;
	
	y = newAngle - kalman->angle;
	S = kalman->P_00 + kalman->R_angle;
	K_0 = kalman->P_00 / S;
	K_1 = kalman->P_11 / S;
	
	kalman->angle +=  K_0 * y;
	kalman->bias  +=  K_1 * y;
	kalman->P_00 -= K_0 * kalman->P_00;
	kalman->P_01 -= K_0 * kalman->P_01;
	kalman->P_10 -= K_1 * kalman->P_00;
	kalman->P_11 -= K_1 * kalman->P_01;
	
	return kalman->angle;
}
/********************************* END OF FILE ********************************/
/******************************************************************************/
