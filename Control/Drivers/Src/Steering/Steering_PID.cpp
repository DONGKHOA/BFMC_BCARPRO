#include "Steering_PID.hpp"

// Reset parameters of PID controller
void Steering_PID:: PID_Reset_Parameters(PID_Steering_parameters *PID_Steering_parameters)
{
    PID_Steering_parameters->error = 0.0f;
    PID_Steering_parameters->pre_error = 0.0f;
    PID_Steering_parameters->pre2_error = 0.0f;
    PID_Steering_parameters->pre_Out = 0.0f;
}

// Set PID Parameters
float Steering_PID:: PID_Set_Parameters(PID_Steering_parameters *PID_Steering_parameters, float Kp, float Ki, float Kd, float PID_saturation, float Ts)
{
    PID_Steering_parameters->Kp = Kp;
    PID_Steering_parameters->Ki = Ki;
    PID_Steering_parameters->Kd = Kd;
    PID_Steering_parameters->PID_saturation = PID_saturation;
    PID_Steering_parameters->Ts = Ts;
}

// PID Controller Process
void Steering_PID:: PID_Process(PID_Steering_parameters *PID_Steering_parameters, float steering)
{
    // Error
    float error;
    error = steering;
    
    // PID Kp, Ki, Kd part
    PID_Steering_parameters->Kp_part = PID_Steering_parameters->Kp * (error - PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Ki_part = 0.5 * PID_Steering_parameters->Ki * PID_Steering_parameters->Ts * (error + PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Kd_part = (PID_Steering_parameters->Kd / PID_Steering_parameters->Ts) * (error - 2*PID_Steering_parameters->pre_error + PID_Steering_parameters->pre2_error);
    
    // Out control
    PID_Steering_parameters->Out = PID_Steering_parameters->pre_Out + PID_Steering_parameters->Kp_part + PID_Steering_parameters->Ki_part + PID_Steering_parameters->Kd_part;
    
    // Set pre Errors
    PID_Steering_parameters->pre_Out = PID_Steering_parameters->Out;
    PID_Steering_parameters->pre2_error = PID_Steering_parameters->pre_error;
    PID_Steering_parameters->pre_error = error;
}

// PID Saturation
float Steering_PID:: PID_Saturation(PID_Steering_parameters *PID_Steering_parameters)
{
    if (PID_Steering_parameters->Out > PID_Steering_parameters->PID_saturation)
    {
        PID_Steering_parameters->Out = PID_Steering_parameters->PID_saturation;
    }
    else if (PID_Steering_parameters->Out < -(PID_Steering_parameters->PID_saturation))
    {
        PID_Steering_parameters->Out = -PID_Steering_parameters->PID_saturation;
    }
}

// Read output control from PID
float Steering_PID:: PID_Read_Value(PID_Steering_parameters *PID_Steering_parameters)
{
    return PID_Steering_parameters->Out;
}

