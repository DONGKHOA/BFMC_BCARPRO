#include "mpu9250_app.h"
#include "stdlib.h"

static float ddt = 1 / 200.f; // Sample rate is 200Hz
extern float LSB_Sensitivity_GYRO;
extern I2C_HandleTypeDef hi2c2;

void MPU9250_DataConvert(imu_9250_t *mpu9250)
{
	mpu9250->data.temperature = (float)(mpu9250->dataraw.temperature_raw) / 340 + 36.53;
	mpu9250->data.gyro_z = (mpu9250->dataraw.gyro_z_raw) / (LSB_Sensitivity_GYRO * 10);
}

void MPU9250_ProcessData(imu_9250_t *mpu9250)
{
	MPU9250_Get6AxisRawData(&(mpu9250->dataraw));
	MPU9250_DataConvert(mpu9250);
}

void CalculateGyroAngle(imu_9250_t *mpu9250)
{
    MPU9250_ProcessData(mpu9250);
    mpu9250->angle.yaw_angle += (mpu9250->data.gyro_z * ddt);
}

void IMU_9250_Init(imu_9250_t *imu_p, void (*get_data_func)(imu_9250_t *imu_p))
{
	imu_p->get_data = get_data_func;
	start_imu();
}

/**
 * The function creates and initializes a new instance of the IMU_9250 structure.
 *
 * @return a pointer to a structure of type imu_9250_t.
 */
imu_9250_t *IMU_9250_Create()
{
	imu_9250_t *imu_p = malloc(sizeof(imu_9250_t));
	if (imu_p != NULL)
	{
		IMU_9250_Init(imu_p, CalculateGyroAngle);
	}
	return imu_p;
}

/**
 * The function IMU_9250_Destroy frees the memory allocated for an imu_9250_t structure.
 *
 * @param imu_p imu_p is a pointer to a structure of type imu_9250_t.
 */
void IMU_9250_Destroy(imu_9250_t *const imu_p)
{
	free(imu_p);
}