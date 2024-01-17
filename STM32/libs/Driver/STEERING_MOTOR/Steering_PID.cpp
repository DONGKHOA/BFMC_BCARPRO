#include "Steering_PID.hpp"

#define KP_VALUE 10.0f;
#define KI_VALUE 10.0f;
#define KD_VALUE 10.0f;
#define TS_VALUE 10.0f;
#define PID_SAT_VALUE 10.0f;

static PID_Steering_parameters parameters;

/* Setup parameter function */
// Reset parameters of PID controller
void Steering_PID_Setup:: PID_Reset_Parameters(PID_Steering_parameters *PID_Steering_parameters)
{
    PID_Steering_parameters->error = 0.0f;
    PID_Steering_parameters->pre_error = 0.0f;
    PID_Steering_parameters->pre2_error = 0.0f;
    PID_Steering_parameters->pre_Out = 0.0f;
}

// Set PID Parameters
float Steering_PID_Setup:: PID_Set_Parameters(PID_Steering_parameters *PID_Steering_parameters)
{
    PID_Steering_parameters->Kp = KP_VALUE;
    PID_Steering_parameters->Ki = KI_VALUE;
    PID_Steering_parameters->Kd = KD_VALUE;
    PID_Steering_parameters->PID_saturation = PID_SAT_VALUE;
    PID_Steering_parameters->Ts = TS_VALUE;
}

void Steering_PID_Setup:: Steering_Setup()
{
    PID_Reset_Parameters(&parameters);
    PID_Set_Parameters(&parameters);
}

// PID Controller Process
void Steering_PID_Control:: PID_Process(PID_Steering_parameters *PID_Steering_parameters, float steering)
{
    // Error
    PID_Steering_parameters->error = steering;
    
    // PID Kp, Ki, Kd part
    PID_Steering_parameters->Kp_part = PID_Steering_parameters->Kp * (error - PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Ki_part = 0.5 * PID_Steering_parameters->Ki * PID_Steering_parameters->Ts * (PID_Steering_parameters->error + PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Kd_part = (PID_Steering_parameters->Kd / PID_Steering_parameters->Ts) * (PID_Steering_parameters->error - 2*PID_Steering_parameters->pre_error + PID_Steering_parameters->pre2_error);
    
    // Out control
    PID_Steering_parameters->Out = PID_Steering_parameters->pre_Out + PID_Steering_parameters->Kp_part + PID_Steering_parameters->Ki_part + PID_Steering_parameters->Kd_part;
    
    // Set pre Errors
    PID_Steering_parameters->pre_Out = PID_Steering_parameters->Out;
    PID_Steering_parameters->pre2_error = PID_Steering_parameters->pre_error;
    PID_Steering_parameters->pre_error = PID_Steering_parameters->error;
}

// PID Saturation
float Steering_PID_Control:: PID_Saturation(PID_Steering_parameters *PID_Steering_parameters)
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
float Steering_PID_Control:: Control_Steering(float distance)
{
    // Take value for process PID
    steering = Read_Steering(distance);
    PID_Process(&parameters, steering);
    PID_Saturation(&parameters);
    // Return Duty cycle
    return parameters.Out;
}

