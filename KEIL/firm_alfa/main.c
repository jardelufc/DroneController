#include	<string.h>						//bibilioteca de manipulação de strings
#include	"inc/pwm.h"						//PWM
#include	"inc/delay.h"					//biblioteca para delay
#include	"inc/USART2.h"				//UART
#include	"inc/MPU6050.h"				//lib comunicacao MPU6050
#include	"inc/controle.h"			//controle do sistema
#include	"stm32f10x.h"					//mapeamento de memoria
#include	"stm32f10x_rcc.h"			//biblioteca para controle de reset e clock

#define BUF_SIZE	16

/* Variaveis -----------------------------------------------------------------*/
uint8_t data[3] = {0,0,0};
uint8_t tmp = 0;
uint8_t buf[BUF_SIZE];
float Racc[3], Rgyro[3];
uint32_t t=0;


/* Funcoes -------------------------------------------------------------------*/
void Clock_Init()
{
	//configura clock da placa
	RCC_HSEConfig(RCC_HSE_ON);
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_PCLK1Config(RCC_HCLK_Div2);
}

void USART2_IRQHandler()
{
	// Check if the USART2 receive interrupt flag was set
	if (USART_GetITStatus(USART2, USART_IT_RXNE)){
		uint8_t c = USART_ReceiveData(USART2);						// Read received char
																											// Read chars until newline
		
		data[0] = (c&0x07);															//dir
		data[1] = (c >> 3)&0x07;												//pwr
		data[2] = (c >> 6)&0x03;												//func
		
		PWM_pwrcrtl(data[2]);
		PWM_dircrtl(data[0], data[1]);
		PWM_update();
	}
}

int main(void)
{
	Clock_Init();									//Define o clock da placa
	DelayInit();									//Define o delay da placa
	USART2_Init();								//Inicia a USART da placa
	PWM_Init();										//Inicia o Timer como pwm
	PWM_Cali();										//Calibra o pwm do esc
	I2C_Configuration();					//Inicia comunicacao i2c
	MPU6050_init();								//Inicia o MPU6050
	
	while(1){
		t = millis();
		MPU6050_get_gyro_acc();
		MPU6050_get_angle(Racc);
		PWM_update();
	}
}
