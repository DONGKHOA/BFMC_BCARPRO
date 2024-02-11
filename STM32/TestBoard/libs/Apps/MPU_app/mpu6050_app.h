#ifndef __MPU6050_APP_H__
#define __MPU6050_APP_H__


#include "main.h"
#include "mpu6050.h"

#define RADIAN_TO_DEGREE 180/3.141592f

float x_Low, y_Low, z_Low, x_old, y_old, z_old;
const float dtt = (1.0 / 100.0); // sample rate
const float RC = 0.35; //RC = 1.0/(cutofFreq * 2 * M_PI);
const float alpha = dtt / (RC + dtt);

void CalculateAccAngle(Angle_t* Angle, MPU6050_t* MPU6050);

#endif
