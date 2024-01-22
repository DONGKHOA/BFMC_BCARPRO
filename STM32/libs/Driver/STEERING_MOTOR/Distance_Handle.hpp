#ifndef _DISTANCE_HANDLE_HPP_
#define _DISTANCE_HANDLE_HPP_

#include <iostream>

class Distance_Handle
{
private:
    const int SIZE = 2;
    /* Superior Distance */
    const float sup_Distance = 1.0f;
    /* Inferior Distance */
    const float inf_Distance = 1.0f;
    /* Steering_Intersection */
    // const float Steering_Intersection = 21.63224079f;
    const float Steering_Intersection = 21.6f;
    /* Steering_Roundabout */
    const float Steering_Roundabout = 13.29111828f;

public:
    /* Interpolate Distance to find corresponding Angle */
    float Interpolate_Distance(float distance, const float distanceValueP[], const float distanceValueN[], const float steeringValueP[], const float steeringValueN[], int size);
    /*- Normal lane -*/
    float Read_Steering(float distance);
    /*- Intersection -*/
    float ReadSteering_intersection();
    /*- Roundabout -*/
    float ReadSteering_Roundabout();
    /* Coefficient for Distance */
    const float distanceValueP[2] = {7.0, 10.0};
    const float distanceValueN[2] = {-7.0, -10.0};
    /* Coefficient for Angle */
    const float steeringValueP[2] = {15.0, 20.0};
    const float steeringValueN[2] = {-15.0, -20.0};
};

#endif