#include "Distance_Handle.hpp"

/**
 * The function Interpolate_Distance calculates the interpolated steering value based on the given
 * distance and reference arrays.
 * 
 * @param distance The distance parameter represents the input distance value for which we want to
 * interpolate a steering value.
 * @param distanceValueP An array of positive distance values. These values represent the distance at
 * which a specific steering value is desired. The array should be sorted in ascending order.
 * @param distanceValueN An array of negative distance values.
 * @param steeringValueP An array of positive steering values corresponding to the distance values in
 * distanceValueP.
 * @param steeringValueN The parameter `steeringValueN` represents an array of steering values
 * corresponding to negative distance values.
 * @param size The parameter "size" represents the size of the arrays distanceValueP[] and
 * distanceValueN[]. It indicates the number of elements in these arrays.
 * 
 * @return a float value, which represents the interpolated steering value based on the given distance
 * and the arrays of distance values and steering values.
 */
float Distance_Handle::Interpolate_Distance(float distance, const float distanceValueP[], const float distanceValueN[], const float steeringValueP[], const float steeringValueN[], int size)
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
                steering = ((distance - distanceValueP[i]) * steeringValueP[i + 1] + (distanceValueP[i + 1] - distance) * steeringValueP[i]) / (distanceValueP[i + 1] - distanceValueP[i]);
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
                steering = ((distance - distanceValueN[i]) * steeringValueN[i + 1] + (distanceValueN[i + 1] - distance) * steeringValueN[i]) / (distanceValueN[i + 1] - distanceValueN[i]);
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

/**
 * The function Read_Steering calculates the steering value based on the given distance using
 * interpolation.
 * 
 * @param distance The distance parameter represents the current distance value that needs to be used
 * for calculating the steering value.
 * 
 * @return the value of the variable "steering".
 */
float Distance_Handle::Read_Steering(float distance)
{
    float steering;
    steering = Interpolate_Distance(distance, distanceValueP, distanceValueN, steeringValueP, steeringValueN, SIZE);
    return steering;
};

/**
 * The function returns the value of the variable Steering_Intersection.
 * 
 * @return the value of the variable "Steering_Intersection" as a float.
 */
float Distance_Handle::ReadSteering_intersection()
{
    return Steering_Intersection;
}

/**
 * The function returns the value of the variable Steering_Roundabout.
 * 
 * @return the value of the variable "Steering_Roundabout" which is of type float.
 */
float Distance_Handle::ReadSteering_Roundabout()
{
    return Steering_Roundabout;
}