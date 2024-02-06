#include "distance_Handle.h"

float Interpolate_Distance(float distance, const float distanceValueP[], const float distanceValueN[], const float steeringValueP[], const float steeringValueN[], int size)
{
    float steering;
    if (distance > 0)
    {
        if (distance <= distanceValueP[0])
            return 0;
        for (int i = 0; i < size; i++)
        {
            if ((distance >= distanceValueP[i]) && distance <= distanceValueP[i + 1])
            {
                steering = ((distance - distanceValueP[i])*steeringValueP[i + 1] + (distanceValueP[i + 1] - distance)*steeringValueP[i])/(distanceValueP[i + 1] - distanceValueP[i]);
                return steering;
            }
        }
    }
    if (distance < 0)
    {
        if (distance >= distanceValueN[0])
            return 0;
        for (int i = 0; i < size; i++)
        {
            if ((distance <= distanceValueN[i]) && distance >= distanceValueN[i + 1])
            {
                steering = ((distance - distanceValueN[i])*steeringValueN[i + 1] + (distanceValueN[i + 1] - distance)*steeringValueN[i])/(distanceValueN[i + 1] - distanceValueN[i]);
                return steering;
            }
        }
    }

    // Boundary conditions for positive and negative reference values
    if (distance >= distanceValueP[size - 1])
        return steeringValueP[size - 1];
    if (distance <= distanceValueN[size - 1])
        return steeringValueN[size - 1];        
};

float Read_Steering(float distance)
{
    float steering;
    steering = Interpolate_Distance(distance, distanceValueP, distanceValueN, steeringValueP, steeringValueN, SIZE);
    return steering;
};