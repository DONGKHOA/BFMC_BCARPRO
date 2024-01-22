#include "Speeding_PID.hpp"

#define KP_VALUE 10.0f;
#define KI_VALUE 10.0f;
#define KD_VALUE 10.0f;
#define TS_VALUE 10.0f;
#define PID_SAT_VALUE 10.0f;

static PID_Speed_parameters parameters;

/**
 * The function PID_Reset_Parameters resets the error, pre_error, pre2_error, and pre_Out variables in
 * the PID_Speed_parameters structure to 0.0.
 * 
 * @param PID_Speed_parameters PID_Speed_parameters is a pointer to a struct that contains the
 * following variables:
 */
void Speeding_PID_Setup::PID_Reset_Parameters(PID_Speed_parameters *PID_Speed_parameters)
{
    PID_Speed_parameters->error = 0.0f;
    PID_Speed_parameters->pre_error = 0.0f;
    PID_Speed_parameters->pre2_error = 0.0f;
    PID_Speed_parameters->pre_Out = 0.0f;
}

/**
 * The function sets the parameters for a PID controller for speed control.
 * 
 * @param PID_Speed_parameters A pointer to a structure that holds the PID speed control parameters.
 */
void Speeding_PID_Setup::PID_Set_Parameters(PID_Speed_parameters *PID_Speed_parameters)
{
    PID_Speed_parameters->Kp = KP_VALUE;
    PID_Speed_parameters->Ki = KI_VALUE;
    PID_Speed_parameters->Kd = KD_VALUE;
    PID_Speed_parameters->PID_saturation = PID_SAT_VALUE;
    PID_Speed_parameters->Ts = TS_VALUE;
}

/**
 * The function Speeding_Setup initializes and sets up the parameters for a PID controller.
 */
void Speeding_PID_Setup::Speeding_Setup()
{
    PID_Reset_Parameters(&parameters);
    PID_Set_Parameters(&parameters);
}


/**
 * The function implements a PID controller to calculate the output control based on the error between
 * the setpoint and the current speed.
 * 
 * @param PID_Speed_parameters A struct that contains the parameters for the PID controller. It
 * includes the following members:
 * @param speeding The "speeding" parameter represents the current speed of the system or process that
 * you are trying to control using the PID controller.
 * @param setpoint The setpoint is the desired value or target value that the system is trying to
 * achieve. In this case, it represents the desired speed that the system is trying to maintain.
 */
void Speeding_PID_Control::PID_Process(PID_Speed_parameters *PID_Speed_parameters, float speeding, float setpoint)
{
    // Error
    PID_Speed_parameters->error = setpoint - speeding;

    // PID Kp, Ki, Kd part
    PID_Speed_parameters->Kp_part = PID_Speed_parameters->Kp * (PID_Speed_parameters->error - PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Ki_part = 0.5 * PID_Speed_parameters->Ki * PID_Speed_parameters->Ts * (PID_Speed_parameters->error + PID_Speed_parameters->pre_error);
    PID_Speed_parameters->Kd_part = (PID_Speed_parameters->Kd / PID_Speed_parameters->Ts) * (PID_Speed_parameters->error - 2 * PID_Speed_parameters->pre_error + PID_Speed_parameters->pre2_error);

    // Out control
    PID_Speed_parameters->Out = PID_Speed_parameters->pre_Out + PID_Speed_parameters->Kp_part + PID_Speed_parameters->Ki_part + PID_Speed_parameters->Kd_part;

    // Set pre Errors
    PID_Speed_parameters->pre_Out = PID_Speed_parameters->Out;
    PID_Speed_parameters->pre2_error = PID_Speed_parameters->pre_error;
    PID_Speed_parameters->pre_error = PID_Speed_parameters->error;
}


/**
 * The function PID_Saturation limits the output value to a specified saturation limit.
 * 
 * @param PID_Speed_parameters PID_Speed_parameters is a pointer to a struct that contains the
 * following variables:
 */
void Speeding_PID_Control::PID_Saturation(PID_Speed_parameters *PID_Speed_parameters)
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


/**
 * The function Control_Speeding uses a PID controller to control the speeding of a system and returns
 * the duty cycle.
 * 
 * @param setpoint The setpoint is the desired speed that the system should maintain. It is the target
 * value that the system is trying to achieve.
 * @param speeding The "speeding" parameter represents the current speed of the system.
 * 
 * @return the duty cycle value.
 */
float Speeding_PID_Control::Control_Speeding(float setpoint, float speeding)
{
    PID_Process(&parameters, speeding, setpoint);
    PID_Saturation(&parameters);
    // Return Duty cycle
    return parameters.Out;
}