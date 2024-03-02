/*********************
 *      INCLUDES
 *********************/

#include "mpu9250.h"

uint16_t error;
float LSB_Sensitivity_GYRO;
extern I2C_HandleTypeDef hi2c1;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void MPU9250_Writebyte(uint8_t reg_addr, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

void MPU9250_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t *data)
{
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);
}

void MPU9250_Readbyte(uint8_t reg_addr, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

void MPU9250_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t *data)
{
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);
}

void MPU9250_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO)
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
}

void start_imu(void)
{
	HAL_Delay(50);
	uint8_t who_am_i = 0;
	MPU9250_Readbyte(MPU9250_WHO_AM_I, &who_am_i);
	if (who_am_i == 0x71)
	{
		error = 20;
	}
	else
	{
		error = 404;
//		while (1);
	}

	// Reset the whole module before initialization
	MPU9250_Writebyte(MPU9250_PWR_MGMT_1, 0x1 << 7);
	HAL_Delay(100);

	// Power Management setting
	/* Default is sleep mode
	 * necessary to wake up MPU9250*/
	MPU9250_Writebyte(MPU9250_PWR_MGMT_1, 0x00);
	HAL_Delay(50);

	// Sample rate divider
	/*Sample Rate = Gyroscope Output Rate / (1 + SMPRT_DIV) */
	//	MPU9250_Writebyte(MPU9250_SMPRT_DIV, 0x00); // ACC output rate is 1kHz, GYRO output rate is 8kHz
	MPU9250_Writebyte(MPU9250_SMPRT_DIV, 39); // Sample Rate = 200Hz		//**********************
	HAL_Delay(50);

	// FSYNC and DLPF setting
	/*DLPF is set to 0*/
	MPU9250_Writebyte(MPU9250_CONFIG, 0x00); //**********************
	HAL_Delay(50);

	// GYRO FULL SCALE setting
	/*FS_SEL  Full Scale Range
	  0    	+-250 degree/s
	  1		+-500 degree/s
	  2		+-1000 degree/s
	  3		+-2000 degree/s	*/
	uint8_t FS_SCALE_GYRO = 0x03;
	MPU9250_Writebyte(MPU9250_GYRO_CONFIG, FS_SCALE_GYRO << 3);
	HAL_Delay(50);

	MPU9250_Get_LSB_Sensitivity(FS_SCALE_GYRO);
}

void MPU9250_Get6AxisRawData(mpu9250_raw_t *mpu9250_raw)
{
	uint8_t data[14];
	MPU9250_Readbytes(MPU9250_ACCEL_XOUT_H, 14, data);

	mpu9250_raw->temperature_raw = (data[6] << 8) | data[7];
	mpu9250_raw->gyro_z_raw = ((data[12] << 8) | data[13]);
}


