#ifndef _SPEEDING_SETUP_HPP_
#define _SPEEDING_SETUP_HPP_

#include "stdint.h"

class Speeding_Setup
{
    private:
        /** @brief 0 default */
        float zero_default = 0.074568;
        /** @brief 0 default */
        int8_t ms_period = 20;
        /** @brief step_value */
        float step_value = 0.00051;

        /* interpolate the step value based on the speed value */
        float Interpolate(float speed, const float speedValuesP[], const float speedValuesN[], const float stepValues[], int size);

        // Predefined values for steering reference and interpolation
        const float speedValuesP[25] = {4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 26.0, 30.0, 35.0, 40.0, 45.0, 50.0};
        const float speedValuesN[25] = {-4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0, -11.0, -12.0, -13.0, -14.0, -15.0, -16.0, -17.0, -18.0, -19.0, -20.0, -21.0, -22.0, -26.0, -30.0, -35.0, -40.0, -45.0, -50.0};
        const float stepValues[25] = {0.00107, 0.00088, 0.00076, 0.00067, 0.0006, 0.00055, 0.00051, 0.00047, 0.00043, 0.00041, 0.00039, 0.00037, 0.00035, 0.00034, 0.00033, 0.00032,
                                        0.0003, 0.00029, 0.00028, 0.00025, 0.00024, 0.00021, 0.00019, 0.00018, 0.00017};

        /* convert speed value to duty cycle for pwm signal */
        float Conversion(float speed); //angle to duty cycle
}; // class CSpeedingMotor

#endif