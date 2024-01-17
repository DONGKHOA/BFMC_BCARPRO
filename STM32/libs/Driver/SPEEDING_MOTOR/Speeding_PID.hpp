#ifndef _Speeding_PID_Setup_HPP_
#define _Speeding_PID_Setup_HPP_

#include <iostream>

class PID_Speed_parameters
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
};

class Speeding_PID_Setup : public PID_Speed_parameters
{
private:
    /* Function */
    // Reset the parameters
    void PID_Reset_Parameters(PID_Speed_parameters *PID_Speed_parameters);
    // Setting PID parameters
    void PID_Set_Parameters(PID_Speed_parameters *PID_Speed_parameters);
    /* Instance */
    static Speeding_PID_Setup *instance;
    Speeding_PID_Setup()
    {
    }

public:
    /* Constructor Speeding_PID_Setup */
    static Speeding_PID_Setup *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Speeding_PID_Setup();
        }
        return instance;
    }

    /* Function for setup */
    void Speeding_Setup();
};

class Speeding_PID_Control : public PID_Speed_parameters
{
private:
    static Speeding_PID_Control *instance;
    Speeding_PID_Control()
    {
    }

public:
    /* Constructor Speeding_PID_Setup */
    static Speeding_PID_Control *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Speeding_PID_Control();
        }
        return instance;
    }

    /* Function for Control */
    // Processing PID module
    void PID_Process(PID_Speed_parameters *PID_Speed_parameters, float speeding, float setpoint);
    // Saturation
    void PID_Saturation(PID_Speed_parameters *PID_Speed_parameters);
    // Read value
    float Control_Speeding(float SP, float PV);
};
#endif