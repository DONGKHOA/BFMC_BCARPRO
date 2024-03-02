#ifndef __CALCULATEANGELE_H__
#define __CALCULATEANGELE_H__

#include "mpu9250.h"

#define RADIAN_TO_DEGREE 180 / 3.141592f

/**********************
 *     OPERATION
 **********************/

typedef struct imu_9250 imu_9250_t;
struct imu_9250
{
	void (*get_data)(imu_9250_t *const imu_p);

	mpu9250_convert_t data;
	mpu9250_angle_t angle;
	mpu9250_raw_t dataraw;
};

imu_9250_t *IMU_9250_Create();
void IMU_9250_Destroy(imu_9250_t *const imu_p);

#endif
