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

/// @brief
/// @param mpu6050
// output: gia tri raw cua gia toc cac truc
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

/// @brief
/// @param FS_SCALE_GYRO
/// @param FS_SCALE_ACC

void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC)
{
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
// chuyen tu raw sang gia tri m/s2, do/giay
void MPU6050_DataConvert(imu_6050_t *mpu6050)
{
	// printf("LSB_Sensitivity_GYRO: %f, LSB_Sensitivity_ACC: %f\n",LSB_Sensitivity_GYRO,LSB_Sensitivity_ACC);
	mpu6050->pt1_p->acc_x = mpu6050->pt1_p->acc_x_raw / LSB_Sensitivity_ACC;
	mpu6050->pt1_p->acc_y = mpu6050->pt1_p->acc_y_raw / LSB_Sensitivity_ACC;
	mpu6050->pt1_p->acc_z = mpu6050->pt1_p->acc_z_raw / LSB_Sensitivity_ACC;

	mpu6050->pt1_p->temperature = (float)(mpu6050->pt1_p->temperature_raw) / 340 + 36.53;

	mpu6050->pt1_p->gyro_x = mpu6050->pt1_p->gyro_x_raw / LSB_Sensitivity_GYRO;
	mpu6050->pt1_p->gyro_y = mpu6050->pt1_p->gyro_y_raw / LSB_Sensitivity_GYRO;
	mpu6050->pt1_p->gyro_z = mpu6050->pt1_p->gyro_z_raw / LSB_Sensitivity_GYRO;
}

// int MPU6050_DataReady(void)
// {
// 	//old school way
// 	/*
// 	static uint8_t INT_STATE_FLAG = 0;
// 	static uint8_t DATA_RDY_INT_FLAG = 0;
// 	static uint8_t INT_PIN = 0;
// 	INT_PIN = LL_GPIO_IsInputPinSet(MPU6050_INT_PORT, MPU6050_INT_PIN);
// 	if(INT_PIN == 1)
// 	{
// 		MPU6050_Readbyte(MPU6050_INT_STATUS, &INT_STATE_FLAG); //flag cleared automatically within the sensor
// 		DATA_RDY_INT_FLAG = INT_STATE_FLAG & 0x01;
// 		if(DATA_RDY_INT_FLAG == 1)
// 		{
// 			INT_STATE_FLAG = 0; //flag clearing
// 			DATA_RDY_INT_FLAG = 0;
// 			INT_PIN = 0;
// 			return 1;
// 		}
// 	}
// 	return 0;
// 	 */
// 	return HAL_GPIO_ReadPin(MPU6050_INT_PORT, MPU6050_INT_PIN);
// }

// input: con tro de lay gia tri cac truc luu vao struct
void MPU6050_ProcessData(imu_6050_t *mpu6050)
{
	MPU6050_Get6AxisRawData(mpu6050);
	MPU6050_DataConvert(mpu6050);
}

// ham init
void IMU_6050_Init(imu_6050_t *imu_p, void (*get_data_func)(imu_6050_t *imu_p))
{
	imu_p->get_data = get_data_func;

	HAL_Delay(50);
	uint8_t who_am_i = 0;
	MPU6050_Readbyte(MPU6050_WHO_AM_I, &who_am_i);
	if (who_am_i == 0x68)
	{
		//		error = 20;
	}
	else
	{
		//		error = 404;
		while (1)
			;
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
	MPU6050_Writebyte(MPU6050_SMPRT_DIV, 39); // Sample Rate = 200Hz
	HAL_Delay(50);

	// FSYNC and DLPF setting
	/*DLPF is set to 0*/
	MPU6050_Writebyte(MPU6050_CONFIG, 0x00);
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

imu_6050_t *IMU_6050_Create()
{
	imu_6050_t *imu_p = malloc(sizeof(imu_6050_t));
	if (imu_p != NULL)
	{
		IMU_6050_Init(imu_p, MPU6050_ProcessData);
	}
	return imu_p;
}

void IMU_6050_Destroy(imu_6050_t *const imu_p)
{
	free(imu_p);
}
