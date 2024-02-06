#ifndef __DRIVERS_IMU_MPU6050_H_
#define __DRIVERS_IMU_MPU6050_H_

/*********************
 *      INCLUDES
 *********************/

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

#define gn 9.80665f

/**********************
 *      TYPEDEFS
 **********************/

extern I2C_HandleTypeDef hi2c1;
uint16_t error;
struct_MPU6050_t MPU6050;
struct_Angle_t Angle;
typedef struct mpu_6050 mpu_6050_t;

static float LSB_Sensitivity_ACC;
static float LSB_Sensitivity_GYRO;


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
} struct_MPU6050_t;




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
} struct_Angle_t;

struct mpu_6050
{
    direction_mpu6050_t direction;
    TIM_HandleTypeDef *timer_p;
    uint32_t timChannel;
	I2C_HandleTypeDef *hi2c;
};
/**********************
 *     OPERATION
 **********************/

void IMU_Init(mpu_6050_t * const mpu_p, TIM_HandleTypeDef *timer_p, uint32_t timChannel, I2C_HandleTypeDef *hi2c);
mpu_6050_t *IMU_Create(TIM_HandleTypeDef * timer_p, uint32_t timChannel, I2C_HandleTypeDef *hi2c);
void IMU_Destroy(mpu_6050_t * const mpu_p);

void CalculateAccAngle(struct_Angle_t* Angle, struct_MPU6050_t* MPU6050);
void MPU6050_Writebyte(uint8_t reg_addr, uint8_t val);
void MPU6050_Writebytes(uint8_t reg_addr, uint8_t len, uint8_t* data);
void MPU6050_Readbyte(uint8_t reg_addr, uint8_t* data);
void MPU6050_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data);
void MPU6050_Initialization(void);
void MPU6050_Get6AxisRawData(struct_MPU6050_t* mpu6050);
int MPU6050_DataReady(void);
void MPU6050_Get_LSB_Sensitivity(uint8_t FS_SCALE_GYRO, uint8_t FS_SCALE_ACC);
void MPU6050_DataConvert(struct_MPU6050_t* mpu6050);
void MPU6050_ProcessData(struct_MPU6050_t* mpu6050);

#endif 