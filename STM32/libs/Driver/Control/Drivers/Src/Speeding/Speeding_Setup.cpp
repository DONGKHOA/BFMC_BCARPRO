#include "Speeding_Setup.hpp"

float Setup_Speeding::Interpolate(float speed, const float speedValuesP[], const float speedValuesN[], const float stepValues[], int size)
{
    if(speed <= speedValuesP[0]){
        if (speed >= speedValuesN[0])
        {
            return stepValues[0];
        }
        else{
            for(int i=1; i<size; i++)
            {
                if (speed >= speedValuesN[i])
                {
                    float slope = (stepValues[i] - stepValues[i-1]) / (speedValuesN[i] - speedValuesN[i-1]);
                    return stepValues[i-1] + slope * (speed - speedValuesN[i-1]);
                }
            }
        }
        
    } 
    if(speed >= speedValuesP[size-1]) return stepValues[size-1];
    if(speed <= speedValuesN[size-1]) return stepValues[size-1];

    for(int i=1; i<size; i++)
    {
        if (speed <= speedValuesP[i])
        {
            float slope = (stepValues[i] - stepValues[i-1]) / (speedValuesP[i] - speedValuesP[i-1]);
            return stepValues[i-1] + slope * (speed - speedValuesP[i-1]);
        }
    }

    return -1;
}

float Setup_Speeding::Conversion(float speed)
{   
    return (step_value * speed + zero_default);
};

