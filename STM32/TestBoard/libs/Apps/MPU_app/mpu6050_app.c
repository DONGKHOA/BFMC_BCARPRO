#include "mpu6050_app.h"

void CalculateAccAngle(Angle_t* Angle, MPU6050_t* MPU6050)
{
	Angle->acc_roll  = atan(-MPU6050->acc_x / sqrt(pow(MPU6050->acc_y,2) + pow(MPU6050->acc_z,2))) * RADIAN_TO_DEGREE;
	Angle->acc_pitch = atan(MPU6050->acc_y / sqrt(pow(MPU6050->acc_x,2) + pow(MPU6050->acc_z,2))) * RADIAN_TO_DEGREE;
	// Angle->acc_yaw = atan(sqrt(pow(MPU6050->acc_x, 2) + pow(MPU6050->acc_y, 2)) / MPU6050->acc_z) * RADIAN_TO_DEGREE;
	//Can't use Angle->acc_yaw there is no reliability. It's based on my personal experimental view.
    x_Low = ((alpha * MPU6050->acc_x) + (1.0 - alpha) * x_old);
    y_Low = ((alpha * MPU6050->acc_y) + (1.0 - alpha) * y_old);
    z_Low = ((alpha * MPU6050->acc_z) + (1.0 - alpha) * z_old);

    x_old = x_Low;
    y_old = y_Low;
    z_old = z_Low;

    Angle->Filt_roll  = atan(-x_Low / sqrt(pow(y_Low,2) + pow(z_Low,2))) * RADIAN_TO_DEGREE;
	Angle->Filt_pitch = atan(y_Low / sqrt(pow(x_Low,2) + pow(z_Low,2))) * RADIAN_TO_DEGREE;
}