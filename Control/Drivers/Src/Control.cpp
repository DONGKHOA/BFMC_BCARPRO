#include "Control.hpp"

namespace drivers
{
    /* Steering */ 
    // Read the duty cycle for Servo
    void Steering:: Steering_Setup(PID_Steering_parameters *PID_Steering_parameters,
                                const float Kp_value, 
                                const float Ki_value, 
                                const float Kd_value, 
                                const float PID_sat_value, 
                                const float Ts)
    {
        // Reset and Setup PID parameters
        PID_Reset_Parameters(PID_Steering_parameters);
        PID_Set_Parameters(PID_Steering_parameters, Kp_value, Ki_value, Kd_value, PID_sat_value, Ts);
    };

    float Steering:: Steering_Control(PID_Steering_parameters *PID_Steering_parameters, float distance)
    {
        float dutyCycle;
        // Take value for process PID
        steering = SetSteering(distance);
        PID_Process(PID_Steering_parameters, steering);
        PID_Saturation(PID_Steering_parameters);
        dutyCycle = PID_Read_Value(PID_Steering_parameters);
    };

    /* Speeding */ 
    // Setup PID and IMU parameters 
    void Speeding:: Speeding_Setup(PID_Speed_parameters *PID_Speed_parameters, 
                                const float Kp_value, 
                                const float Ki_value, 
                                const float Kd_value, 
                                const float PID_sat_value, 
                                const float Ts)
    {
        // Reset and Setup PID parameters 
        PID_Reset_Parameters(PID_Speed_parameters);
        PID_Set_Parameters(PID_Speed_parameters, Kp_value, Ki_value, Kd_value, PID_sat_value, Ts);
        // Reset and Setup IMU parameters
        
        // Take values for Process PID
        PV = Speeding_Read_IMU();
    }

    // Read the duty cycle for BLDC
    float Speeding:: Speeding_Control(PID_Speed_parameters *PID_Speed_parameters, float SP, float PV)
    {
        float dutyCycle;
        // Process PID
        PID_Process(PID_Speed_parameters, SP, PV);
        PID_Saturation(PID_Speed_parameters);
        dutyCycle = PID_Read_Value(PID_Speed_parameters);
        return dutyCycle;
    };
};