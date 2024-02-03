#ifndef __MPU6050_APP_H__
#define __MPU6050_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "mpu6050.h"

#define RADIAN_TO_DEGREE 180/3.141592f

float x_Low, y_Low, z_Low, x_old, y_old, z_old;
const float dtt = (1.0 / 100.0); // sample rate
const float RC = 0.35;
const float alpha = dtt / (RC + dtt);


#ifdef __cplusplus
}
#endif
#endif
