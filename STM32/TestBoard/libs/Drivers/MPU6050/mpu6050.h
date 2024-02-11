#ifndef DRIVERS_MPU_6050_H_
#define DRIVERS_MPU_6050_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define MPU6050_ADDR 0xD0
#define MPU6050_SMPRT_DIV 0X19
#define MPU6050_WHO_AM_I 0X75
#define MPU6050_CONFIG 0X1A
#define MPU6050_GYRO_CONFIG 0X1B
#define MPU6050_ACCEL_CONFIG 0X1C
#define MPU6050_INT_PIN_CFG 0X37
#define MPU6050_INT_ENABLE 0X38
#define MPU6050_INT_STATUS 0X3A
#define MPU6050_ACCEL_XOUT_H 0X3B
#define MPU6050_ACCEL_XOUT_L 0X3C
#define MPU6050_PWR_MGMT_1 0X6B //most important
#define MPU6050_INT_PORT 	GPIOB
#define MPU6050_INT_PIN 	GPIO_PIN_5

/**********************
 *      TYPEDEFS
 **********************/
//float LSB_Sensitivity_ACC;
//float LSB_Sensitivity_GYRO;
//uint16_t error;
extern I2C_HandleTypeDef hi2c1;

typedef struct _MPU6050{
	short acc_x_raw;
	short acc_y_raw;
	short acc_z_raw;
	short temperature_raw;
	short gyro_x_raw;
	short gyro_y_raw;
	short gyro_z_raw;

	float acc_x;
	float acc_y;
	float acc_z;
	float temperature;
	float gyro_x;
	float gyro_y;
	float gyro_z;
} MPU6050_t;




typedef struct _Angle{
	float acc_roll;
	float acc_pitch;
	float acc_yaw;

	float gyro_roll;
	float gyro_pitch;
	float gyro_yaw;

	float ComFilt_roll;
	float ComFilt_pitch;
	float ComFilt_yaw;


	float acc_roll_filt;
	float acc_pitch_filt;
	float acc_yaw_filt;

    float Filt_roll;
	float Filt_pitch;
	float Filt_yaw;
} Angle_t;




// struct imu_6050;
 typedef struct imu_6050 imu_6050_t;

struct imu_6050
{
//	I2C_HandleTypeDef *hi2c;
    void (* get_data)(imu_6050_t *const imu_p);
	MPU6050_t *pt1_p;
	Angle_t *pt2_p;
};


/**********************
 *     OPERATION
 **********************/

imu_6050_t *IMU_6050_Create();
void IMU_6050_Destroy(imu_6050_t * const imu_p);

#endif /* DRIVERS_MPU_6050_H_ */
