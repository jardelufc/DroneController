#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
	 #include	"stm32f10x.h"
	 
	 #define		pwm_period	20000
	 #define		PWM_max			2000
	 #define		PWM_step		15
	 #define		PWM_min			1000
	 #define		TIMx				TIM4
	 
	 void PWM_Init(void);
	 void PWM_Cali(void);
	 void PWM_pwrcrtl(uint8_t tmp);
	 void PWM_dircrtl(uint8_t dir, uint8_t pwr);
	 //void PWM_pid(PWMStruct* PWM_struct, uint16_t dt);
	 void PWM_update(void);
	 
#ifdef __cplusplus
}
#endif

#endif
