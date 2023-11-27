#include "PID.hpp"

// Reset parameters of PID controller
void PID_Controller:: PID_Reset_Parameters(PID_parameters *pid_parameters)
{
    pid_parameters->error = 0.0f;
    pid_parameters->pre_error = 0.0f;
    pid_parameters->pre2_error = 0.0f;
    pid_parameters->pre_Out = 0.0f;
}

// Set PID Parameters
float PID_Controller:: PID_Set_Parameters(PID_parameters *pid_parameters, float Kp, float Ki, float Kd, float PID_saturation, float Ts)
{
    pid_parameters->Kp = Kp;
    pid_parameters->Ki = Ki;
    pid_parameters->Kd = Kd;
    pid_parameters->PID_saturation = PID_saturation;
    pid_parameters->Ts = Ts;
}

// PID Controller Process
void PID_Controller:: PID_Process(PID_parameters *pid_parameters, float velocity, float setpoint)
{
    // Error
    error = setpoint - velocity;
    
    // PID Kp, Ki, Kd part
    pid_parameters->Kp_part = pid_parameters->Kp * (error - pid_parameters->pre_error);
    pid_parameters->Ki_part = 0.5 * pid_parameters->Ki * pid_parameters->Ts * (error + pid_parameters->pre_error);
    pid_parameters->Kd_part = (pid_parameters->Kd / pid_parameters->Ts) * (error - 2*pid_parameters->pre_error + pid_parameters->pre2_error);
    
    // Out control
    pid_parameters->Out = pid_parameters->pre_Out + pid_parameters->Kp_part + pid_parameters->Ki_part + pid_parameters->Kd_part;
    pid_parameters->pre_Out = pid_parameters->Out;
    pid_parameters->pre2_error = pid_parameters->pre_error;
    pid_parameters->pre_error = error;
}

// PID Saturation
float PID_Controller:: PID_Saturation(PID_parameters *pid_parameters)
{
    if (pid_parameters->Out > pid_parameters->PID_saturation)
    {
        pid_parameters->Out = pid_parameters->PID_saturation;
    }
    else if (pid_parameters->Out < -(pid_parameters->PID_saturation))
    {
        pid_parameters->Out = -pid_parameters->PID_saturation;
    }
}

// Read output control from PID
float PID_Controller:: PID_Read_Value(PID_parameters *pid_parameters)
{
    return pid_parameters->Out;
}