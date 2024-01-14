#ifndef _STEERING_SETUP_HPP_
#define _STEERING_SETUP_HPP_

#include "stdint.h"
#include "utility"

class Steering_Setup
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
        /* Saturation of Distance */
        float DistanceSaturation(float distance, const float sup_Distance, const float inf_Distance);
        /* Interpolate Distance to find corresponding Angle */
        float InterpolateDistance(float distance, const float distanceValueP[], const float distanceValueN[], const float steeringValueP[], const float steeringValueN[], int size);
        /*- Normal lane -*/
        float SetSteering(float distance);
        /*- Intersection -*/
        float SetSteering_intersection(float steering);
        /*- Roundabout -*/
        float SetSteering_Roundabout(float steering);
        /* Coefficient for Distance */
        const float distanceValueP[2] = {7.0, 10.0};
        const float distanceValueN[2] = {-7.0, -10.0};
        /* Coefficient for Angle */
        const float steeringValueP[2] = {15.0, 20.0};
        const float steeringValueN[2] = {-15.0, -20.0};
};

#endif