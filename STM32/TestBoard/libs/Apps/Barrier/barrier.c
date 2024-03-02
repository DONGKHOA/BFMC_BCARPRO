#include "barrier.h"

float BARRIER_CONTROL_Set_control(control_barrier_t * control_p)
{

}

void BARRIER_CONTROL_Init(control_p, BARRIER_CONTROL_Set_control)
{

}

control_barrier_t *PID_CONTROL_Create()
{
    control_barrier_t *control_p = malloc(sizeof(control_barrier_t));
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
