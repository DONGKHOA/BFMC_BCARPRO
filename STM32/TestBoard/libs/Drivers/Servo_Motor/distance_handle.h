#ifndef _DISTANCE_HANDLE_H_
#define _DISTANCE_HANDLE_H_

const int SIZE = 2;
const float distanceValueP[2] = {7.0, 10.0};
const float distanceValueN[2] = {-7.0, -10.0};
/* Coefficient for Angle */
const float steeringValueP[2] = {15.0, 20.0};
const float steeringValueN[2] = {-15.0, -20.0};

/* Interpolate Distance to find corresponding Angle */
float Interpolate_Distance(float distance, const float distanceValueP[], const float distanceValueN[], const float steeringValueP[], const float steeringValueN[], int size);
/*- Normal lane -*/
float Read_Steering(float distance);

#endif