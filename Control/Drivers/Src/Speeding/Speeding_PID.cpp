#include "Speeding_PID.hpp"
// Reset parameters of PID controller
void Speeding_PID:: PID_Reset_Parameters(PID_Speed_parameters *PID_Speed_parameters)
{
    PID_Speed_parameters->error = 0.0f;
    PID_Speed_parameters->pre_error = 0.0f;
    PID_Speed_parameters->pre2_error = 0.0f;
    PID_Speed_parameters->pre_Out = 0.0f;
}

// Set PID Parameters
float Speeding_PID:: PID_Set_Parameters(PID_Speed_parameters *PID_Speed_parameters, float Kp, float Ki, float Kd, float PID_saturation, float Ts)
{
    PID_Speed_parameters->Kp = Kp;
    PID_Speed_parameters->Ki = Ki;
    PID_Speed_parameters->Kd = Kd;
    PID_Speed_parameters->PID_saturation = PID_saturation;
    PID_Speed_parameters->Ts = Ts;
}

// PID Controller Process
void Speeding_PID:: PID_Process(PID_Speed_parameters *PID_Speed_parameters, float velocity, float setpoint)
{
    // Error
    error = setpoint - velocity;
    
    // PID Kp, Ki, Kd part
    PID_Speed_parameters->Kp_part = PID_Speed_parameters->Kp * (error - PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Ki_part = 0.5 * PID_Speed_parameters->Ki * PID_Speed_parameters->Ts * (error + PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Kd_part = (PID_Speed_parameters->Kd / PID_Speed_parameters->Ts) * (error - 2*PID_Speed_parameters->pre_error + PID_Speed_parameters->pre2_error);
    
    // Out control
    PID_Speed_parameters->Out = PID_Speed_parameters->pre_Out + PID_Speed_parameters->Kp_part + PID_Speed_parameters->Ki_part + PID_Speed_parameters->Kd_part;
    
    // Set pre Errors
    PID_Speed_parameters->pre_Out = PID_Speed_parameters->Out;
    PID_Speed_parameters->pre2_error = PID_Speed_parameters->pre_error;
    PID_Speed_parameters->pre_error = error;
}

// PID Saturation
float Speeding_PID:: PID_Saturation(PID_Speed_parameters *PID_Speed_parameters)
{
    if (PID_Speed_parameters->Out > PID_Speed_parameters->PID_saturation)
    {
        PID_Speed_parameters->Out = PID_Speed_parameters->PID_saturation;
    }
    else if (PID_Speed_parameters->Out < -(PID_Speed_parameters->PID_saturation))
    {
        PID_Speed_parameters->Out = -PID_Speed_parameters->PID_saturation;
    }
}

// Read output control from PID
float Speeding_PID:: PID_Read_Value(PID_Speed_parameters *PID_Speed_parameters)
{
    return PID_Speed_parameters->Out;
}


