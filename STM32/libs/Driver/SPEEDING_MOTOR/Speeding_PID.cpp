#include "Speeding_PID.hpp"

#define KP_VALUE 10.0f;
#define KI_VALUE 10.0f;
#define KD_VALUE 10.0f;
#define TS_VALUE 10.0f;
#define PID_SAT_VALUE 10.0f;

static PID_Speed_parameters parameters;

// Reset parameters of PID controller
void Speeding_PID_Setup:: PID_Reset_Parameters(PID_Speed_parameters *PID_Speed_parameters)
{
    PID_Speed_parameters->error = 0.0f;
    PID_Speed_parameters->pre_error = 0.0f;
    PID_Speed_parameters->pre2_error = 0.0f;
    PID_Speed_parameters->pre_Out = 0.0f;
}

// Set PID Parameters
void Speeding_PID_Setup:: PID_Set_Parameters(PID_Speed_parameters *PID_Speed_parameters)
{
    PID_Speed_parameters->Kp = KP_VALUE;
    PID_Speed_parameters->Ki = KI_VALUE;
    PID_Speed_parameters->Kd = KD_VALUE;
    PID_Speed_parameters->PID_saturation = PID_SAT_VALUE;
    PID_Speed_parameters->Ts = TS_VALUE;
}

void Speeding_PID_Setup:: Speeding_Setup()
{
    PID_Reset_Parameters(&parameters);
    PID_Set_Parameters(&parameters);
}


// PID Controller Process
void Speeding_PID_Control:: PID_Process(PID_Speed_parameters *PID_Speed_parameters, float speeding, float setpoint)
{
    // Error
    PID_Speed_parameters->error = setpoint - speeding;
    
    // PID Kp, Ki, Kd part
    PID_Speed_parameters->Kp_part = PID_Speed_parameters->Kp * (PID_Speed_parameters->error - PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Ki_part = 0.5 * PID_Speed_parameters->Ki * PID_Speed_parameters->Ts * (PID_Speed_parameters->error + PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Kd_part = (PID_Speed_parameters->Kd / PID_Speed_parameters->Ts) * (PID_Speed_parameters->error - 2*PID_Speed_parameters->pre_error + PID_Speed_parameters->pre2_error);
    
    // Out control
    PID_Speed_parameters->Out = PID_Speed_parameters->pre_Out + PID_Speed_parameters->Kp_part + PID_Speed_parameters->Ki_part + PID_Speed_parameters->Kd_part;
    
    // Set pre Errors
    PID_Speed_parameters->pre_Out = PID_Speed_parameters->Out;
    PID_Speed_parameters->pre2_error = PID_Speed_parameters->pre_error;
    PID_Speed_parameters->pre_error = PID_Speed_parameters->error;
}

// PID Saturation
void Speeding_PID_Control:: PID_Saturation(PID_Speed_parameters *PID_Speed_parameters)
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
float Speeding_PID_Control:: Control_Speeding(float setpoint, float speeding)
{
    PID_Process(&parameters, speeding, setpoint);
    PID_Saturation(&parameters);
    // Return Duty cycle
    return parameters.Out;
}