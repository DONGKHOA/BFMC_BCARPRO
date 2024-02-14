/*********************
 *      INCLUDES
 *********************/

#include "mpu6050.h"
#include <stdlib.h>

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
float LSB_Sensitivity_ACC;
float LSB_Sensitivity_GYRO;
void MPU6050_Writebyte(uint8_t reg_addr, uint8_t val)

{
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, 1);
	//	HAL_I2C_Mem_Write(&(imu_p->hi2c), MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, 1);
}

void MPU6050_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t *data)

{
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1);
	//	HAL_I2C_Mem_Write(&(imu_p->hi2c), MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1);
}

void MPU6050_Readbyte(uint8_t reg_addr, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 1, 1);
	//	HAL_I2C_Mem_Read(&(imu_p->hi2c), MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 1, 1);
}

void MPU6050_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1);
	//	HAL_I2C_Mem_Read(&(imu_p->hi2c), MPU6050_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1);
}

/**
 * The function `MPU6050_Get6AxisRawData` reads raw data from the MPU6050 sensor and stores it in the
 * `imu_6050_t` structure.
 * 
 * @param mpu6050 The parameter "mpu6050" is a pointer to a structure of type "imu_6050_t". This
 * structure likely contains variables to store the raw data values for accelerometer, gyroscope, and
 * temperature readings from the MPU6050 sensor.
 */
void MPU6050_Get6AxisRawData(imu_6050_t *mpu6050)
{
	uint8_t data[14];
	MPU6050_Readbytes(MPU6050_ACCEL_XOUT_H, 14, data);

	mpu6050->pt1_p->acc_x_raw = (data[0] << 8) | data[1];
	mpu6050->pt1_p->acc_y_raw = (data[2] << 8) | data[3];
	mpu6050->pt1_p->acc_z_raw = (data[4] << 8) | data[5];

	mpu6050->pt1_p->temperature_raw = (data[6] << 8) | data[7];

	mpu6050->pt1_p->gyro_x_raw = ((data[8] << 8) | data[9]);
	mpu6050->pt1_p->gyro_y_raw = ((data[10] << 8) | data[11]);
	mpu6050->pt1_p->gyro_z_raw = ((data[12] << 8) | data[13]);
}

/**
 * The function calculates the LSB sensitivity for the MPU6050 gyroscope and accelerometer based on the
 * given full-scale range values.
 * 
 * @param FS_SCALE_GYRO FS_SCALE_GYRO is a parameter that represents the full-scale range for the
 * gyroscope. It determines the sensitivity of the gyroscope measurements. The possible values for
 * FS_SCALE_GYRO are 0, 1, 2, and 3, representing different sensitivity levels.
 * @param FS_SCALE_ACC FS_SCALE_ACC is a parameter that represents the full-scale range for the
 * accelerometer. It determines the sensitivity of the accelerometer to measure changes in
 * acceleration. The value of FS_SCALE_ACC can be 0, 1, 2, or 3, representing different full-scale
 * ranges for the accelerometer.
 */
void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC)
{
	switch (FS_SCALE_GYRO)
		switch (FS_SCALE_GYRO)
		{
		case 0:
			LSB_Sensitivity_GYRO = 131.f;
			break;
		case 1:
			LSB_Sensitivity_GYRO = 65.5f;
			break;
		case 2:
			LSB_Sensitivity_GYRO = 32.8f;
			break;
		case 3:
			LSB_Sensitivity_GYRO = 16.4f;
			break;
		}
	switch (FS_SCALE_ACC)
		switch (FS_SCALE_ACC)
		{
		case 0:
			LSB_Sensitivity_ACC = 16384.f;
			break;
		case 1:
			LSB_Sensitivity_ACC = 8192.f;
			break;
		case 2:
			LSB_Sensitivity_ACC = 4096.f;
			break;
		case 3:
			LSB_Sensitivity_ACC = 2048.f;
			break;
		}
}

/*Convert Unit. acc_raw -> g, gyro_raw -> degree per second*/

/**
 * The function MPU6050_DataConvert converts raw data from an MPU6050 sensor to meaningful values for
 * acceleration, temperature, and gyroscope readings.
 * 
 * @param mpu6050 mpu6050 is a pointer to a structure of type imu_6050_t. This structure contains
 * various fields such as acc_x_raw, acc_y_raw, acc_z_raw, temperature_raw, gyro_x_raw, gyro_y_raw,
 * gyro_z_raw, acc_x, acc_y, acc_z
 */
void MPU6050_DataConvert(imu_6050_t *mpu6050)
{
	// printf("LSB_Sensitivity_GYRO: %f, LSB_Sensitivity_ACC: %f\n",LSB_Sensitivity_GYRO,LSB_Sensitivity_ACC);
	mpu6050->pt1_p->acc_x = mpu6050->pt1_p->acc_x_raw / LSB_Sensitivity_ACC;
	mpu6050->pt1_p->acc_y = mpu6050->pt1_p->acc_y_raw / LSB_Sensitivity_ACC;
	mpu6050->pt1_p->acc_z = mpu6050->pt1_p->acc_z_raw / LSB_Sensitivity_ACC;

	mpu6050->pt1_p->temperature = (float)(mpu6050->pt1_p->temperature_raw) / 340 + 36.53;
	mpu6050->pt1_p->temperature = (float)(mpu6050->pt1_p->temperature_raw) / 340 + 36.53;

	mpu6050->pt1_p->gyro_x = mpu6050->pt1_p->gyro_x_raw / LSB_Sensitivity_GYRO;
	mpu6050->pt1_p->gyro_y = mpu6050->pt1_p->gyro_y_raw / LSB_Sensitivity_GYRO;
	mpu6050->pt1_p->gyro_z = mpu6050->pt1_p->gyro_z_raw / LSB_Sensitivity_GYRO;
}

// input: con tro de lay gia tri cac truc luu vao struct
/**
 * The function MPU6050_ProcessData processes raw data from the MPU6050 sensor by calling two other
 * functions.
 * 
 * @param mpu6050 The parameter "mpu6050" is a pointer to a structure of type "imu_6050_t". This
 * structure likely contains variables and/or arrays to store the raw data and converted data from the
 * MPU6050 sensor.
 */
void MPU6050_ProcessData(imu_6050_t *mpu6050)
{
	MPU6050_Get6AxisRawData(mpu6050);
	MPU6050_DataConvert(mpu6050);
}

void IMU_6050_Init(imu_6050_t *imu_p, void (*get_data_func)(imu_6050_t *imu_p))
{
	imu_p->get_data = get_data_func;

	HAL_Delay(50);
	uint8_t who_am_i = 0;
	//	printf("Checking MPU6050...\n");

	MPU6050_Readbyte(MPU6050_WHO_AM_I, &who_am_i);
	if (who_am_i == 0x68)
	{
		// error = 20;
		//		printf("MPU6050 who_am_i = 0x%02x...OK\n", who_am_i);
	}
	else
	{
		// error = 404;
		//		printf("ERROR!\n");
		//		printf("MPU6050 who_am_i : 0x%02x should be 0x68\n", who_am_i);
		while (1)
		{
			//			printf("who am i error. Can not recognize mpu6050\n");
			HAL_Delay(100);
		}
	}

	// Reset the whole module before initialization
	MPU6050_Writebyte(MPU6050_PWR_MGMT_1, 0x1 << 7);
	HAL_Delay(100);

	// Power Management setting
	/* Default is sleep mode
	 * necessary to wake up MPU6050*/
	MPU6050_Writebyte(MPU6050_PWR_MGMT_1, 0x00);
	HAL_Delay(50);

	// Sample rate divider
	/*Sample Rate = Gyroscope Output Rate / (1 + SMPRT_DIV) */
	//	MPU6050_Writebyte(MPU6050_SMPRT_DIV, 0x00); // ACC output rate is 1kHz, GYRO output rate is 8kHz
	MPU6050_Writebyte(MPU6050_SMPRT_DIV, 39); // Sample Rate = 200Hz		//**********************
	HAL_Delay(50);

	// FSYNC and DLPF setting
	/*DLPF is set to 0*/
	MPU6050_Writebyte(MPU6050_CONFIG, 0x00); //**********************
	HAL_Delay(50);

	// GYRO FULL SCALE setting
	/*FS_SEL  Full Scale Range
	  0    	+-250 degree/s
	  1		+-500 degree/s
	  2		+-1000 degree/s
	  3		+-2000 degree/s	*/
	uint8_t FS_SCALE_GYRO = 0x0;
	MPU6050_Writebyte(MPU6050_GYRO_CONFIG, FS_SCALE_GYRO << 3);
	HAL_Delay(50);

	// ACCEL FULL SCALE setting
	/*FS_SEL  Full Scale Range
	  0    	+-2g
	  1		+-4g
	  2		+-8g
	  3		+-16g	*/
	uint8_t FS_SCALE_ACC = 0x0;
	MPU6050_Writebyte(MPU6050_ACCEL_CONFIG, FS_SCALE_ACC << 3);
	HAL_Delay(50);

	MPU6050_Get_LSB_Sensitivity(FS_SCALE_GYRO, FS_SCALE_ACC);
	//	printf("LSB_Sensitivity_GYRO: %f, LSB_Sensitivity_ACC: %f\n",LSB_Sensitivity_GYRO, LSB_Sensitivity_ACC);

	// Interrupt PIN setting
	uint8_t INT_LEVEL = 0x0;																			  // 0 - active high, 1 - active low
	uint8_t LATCH_INT_EN = 0x0;																			  // 0 - INT 50us pulse, 1 - interrupt clear required
	uint8_t INT_RD_CLEAR = 0x1;																			  // 0 - INT flag cleared by reading INT_STATUS, 1 - INT flag cleared by any read operation
	MPU6050_Writebyte(MPU6050_INT_PIN_CFG, (INT_LEVEL << 7) | (LATCH_INT_EN << 5) | (INT_RD_CLEAR << 4)); //
	HAL_Delay(50);

	// Interrupt enable setting
	uint8_t DATA_RDY_EN = 0x1; // 1 - enable, 0 - disable
	MPU6050_Writebyte(MPU6050_INT_ENABLE, DATA_RDY_EN);
	HAL_Delay(50);

	//	printf("MPU6050 setting is finished\n");
}

/**
 * The function creates and initializes a new instance of the IMU_6050 structure.
 *
 * @return a pointer to a structure of type imu_6050_t.
 */
imu_6050_t *IMU_6050_Create()
{
	imu_6050_t *imu_p = malloc(sizeof(imu_6050_t));
	if (imu_p != NULL)
	{
		IMU_6050_Init(imu_p, MPU6050_ProcessData);
	}
	return imu_p;
}

/**
 * The function IMU_6050_Destroy frees the memory allocated for an imu_6050_t structure.
 *
 * @param imu_p imu_p is a pointer to a structure of type imu_6050_t.
 */
void IMU_6050_Destroy(imu_6050_t *const imu_p)
{
	free(imu_p);
}
