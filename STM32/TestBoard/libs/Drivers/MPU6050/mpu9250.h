#ifndef DRIVERS_MPU_9250_H_
#define DRIVERS_MPU_9250_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define MPU9250_ADDR 0xD0

#define MPU9250_SMPRT_DIV 0X19
#define MPU9250_WHO_AM_I 0X75
#define MPU9250_CONFIG 0X1A
#define MPU9250_GYRO_CONFIG 0X1B
#define MPU9250_ACCEL_CONFIG 0X1C
#define MPU9250_INT_PIN_CFG 0X37
#define MPU9250_INT_ENABLE 0X38
#define MPU9250_INT_STATUS 0X3A
#define MPU9250_ACCEL_XOUT_H 0X3B
#define MPU9250_TEMP_OUT_H 0X41
#define MPU9250_TEMP_OUT_L 0X42
#define MPU9250_GYRO_ZOUT_H 0X47
#define MPU9250_GYRO_ZOUT_L 0X48
#define MPU9250_PWR_MGMT_1 0X6B // most important
// #define MPU9250_INT_PIN_CFG             0x37        /*!< Interrupt pin/bypass enable configuration */
// #define MPU9250_ADDR                    (0x68<<1)   /*!< MPU9250 Address */

/**********************
 *      TYPEDEFS
 **********************/

typedef struct _MPU9250_raw
{
	uint16_t gyro_z_raw;

	uint16_t temperature_raw;
} mpu9250_raw_t;

typedef struct _MPU9250_convert
{
	float gyro_z;

	float temperature;
} mpu9250_convert_t;

typedef struct _MPU9250_angle
{
	float yaw_angle;
} mpu9250_angle_t;



void MPU9250_Get6AxisRawData(mpu9250_raw_t *mpu9250_raw);
void start_imu(void);
void MPU9250_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO);

#endif /* DRIVERS_MPU_9250_H_ */
