#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	#define gyro_sensitivity 		131
	#define accel_sensitivity 	16384
	#define PI		3.14159265359
	//#define acc_x_gain 			16585
	//#define acc_y_gain 			15930
	//#define acc_z_gain 			17383		

	#define	SMPLRT_DIV		0x19	// Sample Rate Divider
	#define	CONFIG			0x1A	// Configuration
	#define	GYRO_CONFIG		0x1B	// Gyroscope Configuration
	#define	ACCEL_CONFIG	0x1C  	// Accelerometer Configuration
	#define	PWR_MGMT_1    	0x6B	// thanh ghi cai dat cac che do nhu sleep, chon tan so dao dong ... cua MPU

	#define start_read_address 0x3B
	#define gyro_read_address	0x43
	 
	void I2C_Configuration(void);
	void MPU6050_write_data(uint8_t* pBuffer, uint8_t writeAddr);
	void MPU6050_read_data(uint8_t readAddr, uint8_t* pBuffer, uint8_t len);
	
	void MPU6050_init(void);
	void MPU6050_get_Rgyro(uint16_t* gyro);
	void MPU6050_get_Racc(uint16_t* acc);
	void MPU6050_rawAccToDeg(uint16_t* raw, float* RwAcc);
	void MPU6050_rawGyroToDegsec(uint16_t* raw, float* gyro_ds);
	void MPU6050_normalize(float* vector);
	void MPU6050_process(void);
	void MPU6050_getRest(float* R);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
