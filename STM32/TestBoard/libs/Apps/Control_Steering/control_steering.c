#include "control_steering.h"

/**
 * The function PID_CONTROL_Reset_Parameters resets the error and output parameters of a PID controller
 * structure.
 *
 * @param control_p The `control_p` parameter is a pointer to a structure of type `control_steering_t`. The
 * function `PID_CONTROL_Reset_Parameters` resets the error, pre_error, pre2_error, and pre_Out members
 * of the structure to 0.0.
 */
void PID_CONTROL_Reset_Parameters(control_steering_t *const control_p)
{
    control_p->error = 0.0f;
    control_p->pre_error = 0.0f;
    control_p->pre2_error = 0.0f;
    control_p->pre_Out = 0.0f;
}

/**
 * The function PID_CONTROL_Set_Parameters sets the parameters for a PID controller structure.
 *
 * @param control_p control_p is a pointer to a structure of type control_steering_t.
 */
void PID_CONTROL_Set_Parameters(control_steering_t *const control_p)
{
    control_p->Kp = KP_VALUE;
    control_p->Ki = KI_VALUE;
    control_p->Kd = KD_VALUE;
    control_p->Ts = TS_VALUE;
    control_p->Inf_Saturation = INF_VALUE;
    control_p->Sup_Saturation = SUP_VALUE;
}

/**
 * The function calculates the control output using a PID controller with proportional, integral, and
 * derivative terms, and limits the output within a specified saturation range.
 *
 * @param control_p control_p is a pointer to a structure of type control_steering_t, which contains the
 * following members:
 *
 * @return The function `PID_CONTROL_Set_control` is returning the calculated control output value as a
 * `uint32_t` data type.
 */
uint32_t PID_CONTROL_Set_control(control_steering_t *const control_p)
{
    // Error
	if (reference > 0)
	{
		control_p->error = control_p->reference - fabs(control_p->angle1 - control_p->angle0);
	}
	else if (reference < 0)
	{
		control_p->error = control_p->reference + fabs(control_p->angle1 - control_p->angle0);
	}
    control_p->angle0 = control_p->angle1;
    // PID Kp, Ki, Kd part
    control_p->Kp_part = control_p->Kp * (control_p->error - control_p->pre_error);
    control_p->Ki_part = 0.5 * control_p->Ki * control_p->Ts * (control_p->error + control_p->pre_error);
    control_p->Kd_part = (control_p->Kd / control_p->Ts) * (control_p->error - 2 * control_p->pre_error + control_p->pre2_error);
    // Out control
    control_p->Out = control_p->pre_Out + control_p->Kp_part + control_p->Ki_part + control_p->Kd_part;
    // Set pre Errors
    control_p->pre_Out = control_p->Out;
    control_p->pre2_error = control_p->pre_error;
    control_p->pre_error = control_p->error;
    // Boundary output
    if (control_p->Out > control_p->Inf_Saturation)
    {
        control_p->Out = control_p->Inf_Saturation;
    }
    else if (control_p->Out < control_p->Sup_Saturation)
    {
        control_p->Out = control_p->Sup_Saturation;
    }
    return (uint32_t)control_p->Out;
}

/**
 * The function PID_CONTROL_Init initializes a PID controller with specified parameters and control
 * function.
 *
 * @param control_p The `control_p` parameter is a pointer to a structure of type `control_steering_t`.
 * @param set_control The `set_control` parameter in the `PID_CONTROL_Init` function is a function
 * pointer that points to a function which takes a pointer to a `control_steering_t` structure as an
 * argument and returns a `uint32_t` value. This function is used to set the control parameters for the
 * PID
 */
void PID_CONTROL_Init(control_steering_t * const control_p,
                        uint32_t (*set_control_func)(control_steering_t *const control_p))
{
    control_p->set_control = set_control_func;
    PID_CONTROL_Reset_Parameters(control_p);
    PID_CONTROL_Set_Parameters(control_p);
    control_p->angle1 = 0.0f;
    control_p->angle0 = 0.0f;
    PID_CONTROL_Set_control(control_p);
}

/**
 * The function PID_CONTROL_Create dynamically allocates memory for a control_steering_t structure and
 * initializes it.
 *
 * @return The function `PID_CONTROL_Create()` is returning a pointer to a newly allocated
 * `control_steering_t` structure.
 */
control_steering_t *PID_CONTROL_Create()
{
    control_steering_t *control_p = malloc(sizeof(control_steering_t));
    if (control_p != NULL)
    {
        PID_CONTROL_Init(control_p, PID_CONTROL_Set_control);
    }
    return control_p;
}

/**
 * The function PID_CONTROL_Destroy deallocates memory for a PID control structure.
 *
 * @param control_p The `control_p` parameter is a pointer to a structure of type `control_steering_t`.
 */
void PID_CONTROL_Destroy(control_steering_t * const control_p)
{
    free(control_p);
}
