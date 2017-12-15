#ifndef __CONTROLE_H
#define __CONTROLE_H

#ifdef __cplusplus
 extern "C" {
#endif

	 typedef struct{
		float Q_angle;	//0.001
		float Q_gyro;		//0.003
		float R_angle;	//0.03

		float angle;
		float bias;
		float P_00; 
		float P_01; 
		float P_10; 
		float P_11;
	 }kalmanStruct;

	 #include	"stm32f10x.h"
	 #include "pwm.h"
	 
	 uint16_t map(uint16_t var, uint16_t in_max, uint16_t in_min, uint16_t out_min, uint16_t out_max);
	 void kalman_InitStruct(kalmanStruct* kalman);
	 float kalman_filter(kalmanStruct* kalman, float newAngle, float newRate, uint32_t looptime);
	 uint16_t intencidade(uint8_t grau);

#ifdef __cplusplus
}
#endif

#endif
