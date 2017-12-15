/******************************************************************
Project: MPU6050_with_Flying-KIT
Created: 26/12/2013 9:24:17 SA
Author: NhuongTrieu
******************************************************************/

#include <math.h>
#include "stm32f10x.h"
#include "../inc/delay.h"
#include "../inc/MPU6050.h"


#define MPU6050_ADDR 0xD0
#define MPU6050_I2C I2C2

/*********************************************

Rate                Sensitivity
250 deg/s           131
500 deg/s           65.5
1000 deg/s          32.8
2000 deg/s          16.4

*********************************************/

static int16_t gyro_value[8], gyro_zero[8];
static int32_t gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z;
static int32_t gyro_x_zero = 98;
static int32_t gyro_y_zero = 69;
static int32_t gyro_z_zero = 0;


uint8_t mpu_wdata[1], mpu_rdata[14];

void I2C_Configuration(void)
{
   I2C_InitTypeDef  I2C_Config;
   GPIO_InitTypeDef  GPIO_InitStructure; 

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);

   /* Configure I2C1 pins: PB6->SCL and PB7->SDA */
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
		
   I2C_DeInit(MPU6050_I2C);
   I2C_Config.I2C_Mode = I2C_Mode_I2C;
   I2C_Config.I2C_DutyCycle = I2C_DutyCycle_2;
   I2C_Config.I2C_OwnAddress1 = 0xD0;
   I2C_Config.I2C_Ack = I2C_Ack_Enable;
   I2C_Config.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_Config.I2C_ClockSpeed = 400000;
    
   I2C_Cmd(MPU6050_I2C, ENABLE);
   I2C_Init(MPU6050_I2C, &I2C_Config);

   I2C_AcknowledgeConfig(MPU6050_I2C, ENABLE);
}

void MPU6050_write_data(uint8_t* pBuffer, uint8_t writeAddr)
{
		/* Send START condition */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

		/* Send MPU6050 address for write */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Transmitter);
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		/* Send the MPU6050's internal address to write to */
  	MPU6050_I2C->DR = writeAddr;
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send the byte to be written */
  	MPU6050_I2C->DR = *pBuffer;
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STOP condition */
  	I2C_GenerateSTOP(MPU6050_I2C, ENABLE);
}

void MPU6050_read_data(uint8_t readAddr, uint8_t* pBuffer, uint8_t len)
{
		/* While the bus is busy */
  	while(I2C_GetFlagStatus(MPU6050_I2C, I2C_FLAG_BUSY));

		/* Send START condition */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

		/* Send MPU6050 address for write */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Transmitter); 
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		/* Clear EV6 by setting again the PE bit */
  	I2C_Cmd(MPU6050_I2C, ENABLE);

		/* Send the MPU6050's internal address to write to */
  	MPU6050_I2C->DR = readAddr;
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STRAT condition a second time */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

		/* Send MPU6050 address for read */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Receiver);
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

		/* While there is data to be read */
  	while(len)
  	{
    	if(len == 1)
    	{
      		/* Disable Acknowledgement */
      		I2C_AcknowledgeConfig(MPU6050_I2C, DISABLE);
      		/* Send STOP Condition */
      		I2C_GenerateSTOP(MPU6050_I2C, ENABLE);
    	}

    	/* Test on EV7 and clear it */
    	if(I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    	{
      		*pBuffer = MPU6050_I2C->DR;
      		/* Point to the next location where the byte read will be saved */
      		pBuffer++;
      		/* Decrement the read bytes counter */
      		len--;
    	}
  	}
  	/* Enable Acknowledgement to be ready for another reception */
  	I2C_AcknowledgeConfig(MPU6050_I2C, ENABLE);
}

void MPU6050_init()
{
	mpu_wdata[0]=0x80;  														// Reset device (MPU6050)
	MPU6050_write_data(mpu_wdata, PWR_MGMT_1); 			// write register
	delay_ms(5);

	mpu_wdata[0]=0x00; 															// Enable device (MPU6050)
	MPU6050_write_data(mpu_wdata, PWR_MGMT_1); 			// write register
	delay_ms(5);
	
	mpu_wdata[0]=0x00; 															// SampleRate = gyro_rate_output.
	MPU6050_write_data(mpu_wdata, SMPLRT_DIV); 			// write register
	delay_ms(5);
	
	mpu_wdata[0]=0x07; 															// filter with frequency 8kHz
	MPU6050_write_data(mpu_wdata, CONFIG); 					// write register
	delay_ms(5);
	
	mpu_wdata[0]=0x08;  														// gyro_output range on 500deg/s
	MPU6050_write_data(mpu_wdata, GYRO_CONFIG); 		// write register
	delay_ms(5);
	
	mpu_wdata[0]=0x00; 															// accel_output range on +/- 2g
	MPU6050_write_data(mpu_wdata, ACCEL_CONFIG); 		// write register
	delay_ms(5);
}


void MPU6050_get_gyro_acc(int16_t* AccelGyro) 
{
    uint8_t tmpBuffer[14]; 
		int i;
    MPU6050_read_data(start_read_address, tmpBuffer, 14); 
    /* Get accelation */
    for(i=0; i<3; i++) 
    AccelGyro[i]=(tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1];
		/* Temperature MPU*/
		//AccelGyro[3]=(int16_t)((tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1])/340 + 36.53;
    /* Get Gyro Rate */
    for(i=4; i<7; i++)
    AccelGyro[i]=(tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1];        
}

void MPU6050_get_value()
{
	u16 i=0;
	for(i=0;i<50;i++)
	{
		MPU6050_get_gyro_acc(gyro_value); 
	
		acc_x+=(int32_t)(gyro_value[0]);
		acc_y+=(int32_t)(gyro_value[1]);
		acc_z+=(int32_t)(gyro_value[2]);
		//temp+=(int32_t)(gyro_value[3]);
		gyro_x+=(int32_t)(gyro_value[4]);
		gyro_y+=(int32_t)(gyro_value[5]);
		//gyro_z+=(int32_t)(gyro_value[6]);
	}
	acc_x/=50;
	acc_y/=50;
	acc_z/=50;
	//temp/=50;
	gyro_x/=50;
	gyro_y/=50;
	//gyro_z/=50;
}

double MPU6050_gyro_x_rate()
{
		gyro_x_rate = (float)(gyro_x - gyro_x_zero)/gyro_sensitivity;
		return gyro_x_rate;
}

double MPU6050_gyro_y_rate()
{
		gyro_y_rate = (float)(gyro_y - gyro_y_zero)/gyro_sensitivity;
		return gyro_y_rate;
}

double MPU6050_get_x_angle()
{
	acc_x_angle=(rad_to_degree)*atan((float)acc_y/sqrt((float)(acc_x*acc_x)+(float)(acc_z*acc_z)));
	
	if(acc_z<0)
	{
			if(acc_x_angle<0)
			{
				acc_x_angle=-acc_x_angle;// 0>= acc_x_angle> -180
			}
			else
			{
				acc_x_angle=180-acc_x_angle;//0 =< acc_x_angle =< 180
			}
	}
	return acc_x_angle;
}

double MPU6050_get_y_angle()
{
	acc_y_angle=(rad_to_degree)*atan((float)-acc_x/sqrt((float)(acc_y*acc_y)+(float)(acc_z*acc_z)));
	
	if(acc_z<0)
	{
			if(acc_y_angle<0)
			{
				acc_y_angle=-acc_y_angle;// 0>= acc_x_angle> -180
			}
			else
			{
				acc_y_angle=180-acc_y_angle;//0 =< acc_x_angle =< 180
			}
	}
	return acc_y_angle;
}
