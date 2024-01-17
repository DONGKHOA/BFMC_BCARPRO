#include "Steering_PID.hpp"

#define KP_VALUE 10.0f;
#define KI_VALUE 10.0f;
#define KD_VALUE 10.0f;
#define TS_VALUE 10.0f;
#define PID_SAT_VALUE 10.0f;

static PID_Steering_parameters parameters;

/**
 * The function PID_Reset_Parameters resets the parameters of a PID controller for steering.
 * 
 * @param PID_Steering_parameters A structure that contains the parameters for the PID controller used
 * for steering control.
 */
void Steering_PID_Setup::PID_Reset_Parameters(PID_Steering_parameters *PID_Steering_parameters)
{
    PID_Steering_parameters->error = 0.0f;
    PID_Steering_parameters->pre_error = 0.0f;
    PID_Steering_parameters->pre2_error = 0.0f;
    PID_Steering_parameters->pre_Out = 0.0f;
}

/**
 * The function PID_Set_Parameters sets the values of the PID steering parameters.
 * 
 * @param PID_Steering_parameters PID_Steering_parameters is a pointer to a struct that contains the
 * following parameters:
 */
float Steering_PID_Setup::PID_Set_Parameters(PID_Steering_parameters *PID_Steering_parameters)
{
    PID_Steering_parameters->Kp = KP_VALUE;
    PID_Steering_parameters->Ki = KI_VALUE;
    PID_Steering_parameters->Kd = KD_VALUE;
    PID_Steering_parameters->PID_saturation = PID_SAT_VALUE;
    PID_Steering_parameters->Ts = TS_VALUE;
}

/**
 * The function "Steering_Setup" initializes and sets up the parameters for a PID controller.
 */
void Steering_PID_Setup::Steering_Setup()
{
    PID_Reset_Parameters(&parameters);
    PID_Set_Parameters(&parameters);
}

/**
 * The function implements a PID controller for steering control, calculating the control output based
 * on the current error and previous errors.
 * 
 * @param PID_Steering_parameters PID_Steering_parameters is a struct that contains the following
 * variables:
 * @param steering The "steering" parameter represents the current steering value or error that needs
 * to be controlled. It is the input to the PID controller.
 */
void Steering_PID_Control::PID_Process(PID_Steering_parameters *PID_Steering_parameters, float steering)
{
    // Error
    PID_Steering_parameters->error = steering;

    // PID Kp, Ki, Kd part
    PID_Steering_parameters->Kp_part = PID_Steering_parameters->Kp * (error - PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Ki_part = 0.5 * PID_Steering_parameters->Ki * PID_Steering_parameters->Ts * (PID_Steering_parameters->error + PID_Steering_parameters->pre_error);
    PID_Steering_parameters->Kd_part = (PID_Steering_parameters->Kd / PID_Steering_parameters->Ts) * (PID_Steering_parameters->error - 2 * PID_Steering_parameters->pre_error + PID_Steering_parameters->pre2_error);

    // Out control
    PID_Steering_parameters->Out = PID_Steering_parameters->pre_Out + PID_Steering_parameters->Kp_part + PID_Steering_parameters->Ki_part + PID_Steering_parameters->Kd_part;

    // Set pre Errors
    PID_Steering_parameters->pre_Out = PID_Steering_parameters->Out;
    PID_Steering_parameters->pre2_error = PID_Steering_parameters->pre_error;
    PID_Steering_parameters->pre_error = PID_Steering_parameters->error;
}

/**
 * The function PID_Saturation limits the output value to a specified saturation limit.
 * 
 * @param PID_Steering_parameters A structure that contains the following variables:
 */
float Steering_PID_Control::PID_Saturation(PID_Steering_parameters *PID_Steering_parameters)
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


/**
 * The function Control_Steering uses a PID controller to calculate the duty cycle for steering based
 * on the distance input.
 * 
 * @param distance The "distance" parameter represents the distance between the vehicle and the desired
 * position or target. It is used as an input to calculate the steering control signal.
 * 
 * @return the duty cycle value calculated by the PID controller.
 */
float Steering_PID_Control::Control_Steering(float distance)
{
    // Take value for process PID
    steering = Read_Steering(distance);
    PID_Process(&parameters, steering);
    PID_Saturation(&parameters);
    // Return Duty cycle
    return parameters.Out;
}