#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	#define gyro_sensitivity 65.5  
	#define accel_sensitivity 16384
	#define acc_x_gain 16585
	#define acc_y_gain 15930
	#define acc_z_gain 17383
	#define rad_to_degree 57.29577951308232087679815481

	#define	SMPLRT_DIV		0x19	// Sample Rate Divider
	#define	CONFIG				0x1A	// Configuration
	#define	GYRO_CONFIG		0x1B	// Gyroscope Configuration
	#define	ACCEL_CONFIG	0x1C  // Accelerometer Configuration
	#define	PWR_MGMT_1    0x6B	// thanh ghi cai dat cac che do nhu sleep, chon tan so dao dong ... cua MPU

	#define start_read_address 0x3B
	 
	static float temp;
	static float gyro_x_rate, gyro_y_rate, gyro_z_rate;
	static float acc_x_angle, acc_y_angle, acc_z_angle;
	 
	void I2C_Configuration(void);
	void MPU6050_write_data(uint8_t* pBuffer, uint8_t writeAddr);
	void MPU6050_read_data(uint8_t readAddr, uint8_t* pBuffer, uint8_t len);
	void MPU6050_init(void);
	void MPU6050_get_gyro_acc(int16_t* AccelGyro);
	void MPU6050_get_value(void);
	
	double MPU6050_gyro_x_rate(void);
	double MPU6050_gyro_y_rate(void);
	double MPU6050_get_x_angle(void);
	double MPU6050_get_y_angle(void);
	 
	 
#ifdef __cplusplus
}
#endif

#endif
