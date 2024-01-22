#ifndef _STEERING_PID_HPP_
#define _STEERING_PID_HPP_

#include "Distance_Handle.hpp"

class PID_Steering_parameters
{
public:
    float Kp;
    float Ki;
    float Kd;
    float error;
    float pre_error;
    float pre2_error;
    float Ts;
    float Kp_part;
    float Ki_part;
    float Kd_part;
    float Out;
    float pre_Out;
    float PID_saturation;
    float steering;
};

class Steering_PID_Setup : public PID_Steering_parameters
{
private:
    /* function */
    // Reset the parameters
    void PID_Reset_Parameters(PID_Steering_parameters *PID_Speed_parameters);
    // Setting PID parameters
    float PID_Set_Parameters(PID_Steering_parameters *PID_Steering_parameters);
    /* Instance */
    static Steering_PID_Setup *instance;
    Steering_PID_Setup()
    {
    }

public:
    /* Constructor Speeding_PID_Setup */
    static Steering_PID_Setup *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Steering_PID_Setup();
        }
        return instance;
    }
    /* Function for setup */
    void Steering_Setup();
};

class Steering_PID_Control : public PID_Steering_parameters, public Distance_Handle
{
private:
    static Steering_PID_Control *instance;
    Steering_PID_Control()
    {
    }

public:
    /* Constructor Speeding_PID_Setup */
    static Steering_PID_Control *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Steering_PID_Control();
        }
        return instance;
    }

    /* Function for Control */
    // Processing PID module
    void PID_Process(PID_Steering_parameters *PID_Steering_parameters, float steering);
    // Saturation
    float PID_Saturation(PID_Steering_parameters *PID_Steering_parameters);
    // Read value
    float Control_Steering(float distance);
};

#endif