#include "control_steering.h"
#include "qfis.h"

/* FIS Object */
static qFIS_t Fuzzy;
/* I/O Fuzzy Objects */
static qFIS_Input_t Fuzzy_inputs[2];
static qFIS_Output_t Fuzzy_outputs[1];
/* I/O Membership Objects */
static qFIS_MF_t MFin[ 14 ], MFout[ 7 ];
/* I/O Names */
enum { Angle, Velocity };
enum { Steering };
/* I/O Membership functions tags */
enum { Angle_PB, Angle_NB, Angle_ZO, Angle_PS, Angle_NS, Angle_PM, Angle_NM, Velocity_PS, Velocity_NS, Velocity_PB, Velocity_NB, Velocity_ZO, Velocity_PM, Velocity_NM };
enum { Steering_ZO, Steering_PS, Steering_NS, Steering_PB, Steering_NB, Steering_PM, Steering_NM };
/* Rules of the inference system */
static const qFIS_Rules_t rules[] = {
    QFIS_RULES_BEGIN
       IF Angle IS Angle_PB AND Velocity IS Velocity_PS THEN Steering IS Steering_NB END
       IF Angle IS Angle_NB AND Velocity IS Velocity_PS THEN Steering IS Steering_PM END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_PS THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_PS THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_PS THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_PS THEN Steering IS Steering_NS END
       IF Angle IS Angle_NM AND Velocity IS Velocity_PS THEN Steering IS Steering_PM END
       IF Angle IS Angle_PB AND Velocity IS Velocity_NS THEN Steering IS Steering_NM END
       IF Angle IS Angle_NB AND Velocity IS Velocity_NS THEN Steering IS Steering_PM END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_NS THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_NS THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_NS THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_NS THEN Steering IS Steering_NS END
       IF Angle IS Angle_NM AND Velocity IS Velocity_NS THEN Steering IS Steering_PS END
       IF Angle IS Angle_PB AND Velocity IS Velocity_PB THEN Steering IS Steering_NB END
       IF Angle IS Angle_NB AND Velocity IS Velocity_PB THEN Steering IS Steering_PB END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_PB THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_PB THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_PB THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_PB THEN Steering IS Steering_NM END
       IF Angle IS Angle_NM AND Velocity IS Velocity_PB THEN Steering IS Steering_PM END
       IF Angle IS Angle_PB AND Velocity IS Velocity_NB THEN Steering IS Steering_NB END
       IF Angle IS Angle_NB AND Velocity IS Velocity_NB THEN Steering IS Steering_PB END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_NB THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_NB THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_NB THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_NB THEN Steering IS Steering_NM END
       IF Angle IS Angle_NM AND Velocity IS Velocity_NB THEN Steering IS Steering_PM END
       IF Angle IS Angle_PB AND Velocity IS Velocity_ZO THEN Steering IS Steering_NB END
       IF Angle IS Angle_NB AND Velocity IS Velocity_ZO THEN Steering IS Steering_PB END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_ZO THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_ZO THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_ZO THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_ZO THEN Steering IS Steering_NM END
       IF Angle IS Angle_NM AND Velocity IS Velocity_ZO THEN Steering IS Steering_PM END
       IF Angle IS Angle_PB AND Velocity IS Velocity_PM THEN Steering IS Steering_NM END
       IF Angle IS Angle_NB AND Velocity IS Velocity_PM THEN Steering IS Steering_PM END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_PM THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_PM THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_PM THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_PM THEN Steering IS Steering_NM END
       IF Angle IS Angle_NM AND Velocity IS Velocity_PM THEN Steering IS Steering_PM END
       IF Angle IS Angle_PB AND Velocity IS Velocity_NM THEN Steering IS Steering_NB END
       IF Angle IS Angle_NB AND Velocity IS Velocity_NM THEN Steering IS Steering_PB END
       IF Angle IS Angle_ZO AND Velocity IS Velocity_NM THEN Steering IS Steering_ZO END
       IF Angle IS Angle_PS AND Velocity IS Velocity_NM THEN Steering IS Steering_NS END
       IF Angle IS Angle_NS AND Velocity IS Velocity_NM THEN Steering IS Steering_PS END
       IF Angle IS Angle_PM AND Velocity IS Velocity_NM THEN Steering IS Steering_NM END
       IF Angle IS Angle_NM AND Velocity IS Velocity_NM THEN Steering IS Steering_PM END
    QFIS_RULES_END
};
/* Rule strengths */
float rStrength[ 49 ] = { 0.0f };

/* Parameters of the membership functions */
static const float Angle_PB_p[] = { 0.1953f, 1.0000f };
static const float Angle_NB_p[] = { 0.1953f, -1.0000f };
static const float Angle_ZO_p[] = { 0.1274f, 0.0000f };
static const float Angle_PS_p[] = { 0.1274f, 0.3000f };
static const float Angle_NS_p[] = { 0.1274f, -0.3000f };
static const float Angle_PM_p[] = { 0.1316f, 0.6050f };
static const float Angle_NM_p[] = { 0.1316f, -0.6050f };
static const float Velocity_PS_p[] = { 0.1274f, 0.3000f };
static const float Velocity_NS_p[] = { 0.1274f, -0.3000f };
static const float Velocity_PB_p[] = { 0.1953f, 1.0000f };
static const float Velocity_NB_p[] = { 0.1953f, -1.0000f };
static const float Velocity_ZO_p[] = { 0.1274f, 0.0000f };
static const float Velocity_PM_p[] = { 0.1316f, 0.6050f };
static const float Velocity_NM_p[] = { 0.1316f, -0.6050f };
static const float Steering_ZO_p[] = { 0.1274f, 0.0000f };
static const float Steering_PS_p[] = { 0.1274f, 0.3000f };
static const float Steering_NS_p[] = { 0.1274f, -0.3000f };
static const float Steering_PB_p[] = { 0.1953f, 1.0000f };
static const float Steering_NB_p[] = { 0.1953f, -1.0000f };
static const float Steering_PM_p[] = { 0.1316f, 0.6050f };
static const float Steering_NM_p[] = { 0.1316f, -0.6050f };

void Fuzzy_init(control_steering_t * const control_p,
				uint32_t (*set_control_func)(control_steering_t *const control_p, float distance, uint32_t duty_speed))
{
    /* Set inputs */
    qFIS_InputSetup( Fuzzy_inputs, Angle, -1.0000f, 1.0000f );
    qFIS_InputSetup( Fuzzy_inputs, Velocity, -1.0000f, 1.0000f );
    /* Set outputs */
    qFIS_OutputSetup( Fuzzy_outputs, Steering, -1.0000f, 1.0000f );
    /* Set membership functions for the inputs */
    qFIS_SetMF( MFin, Angle, Angle_PB, gaussmf, NULL, Angle_PB_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_NB, gaussmf, NULL, Angle_NB_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_ZO, gaussmf, NULL, Angle_ZO_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_PS, gaussmf, NULL, Angle_PS_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_NS, gaussmf, NULL, Angle_NS_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_PM, gaussmf, NULL, Angle_PM_p, 1.0f );
    qFIS_SetMF( MFin, Angle, Angle_NM, gaussmf, NULL, Angle_NM_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_PS, gaussmf, NULL, Velocity_PS_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_NS, gaussmf, NULL, Velocity_NS_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_PB, gaussmf, NULL, Velocity_PB_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_NB, gaussmf, NULL, Velocity_NB_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_ZO, gaussmf, NULL, Velocity_ZO_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_PM, gaussmf, NULL, Velocity_PM_p, 1.0f );
    qFIS_SetMF( MFin, Velocity, Velocity_NM, gaussmf, NULL, Velocity_NM_p, 1.0f );
    /* Set membership functions for the outputs */
    qFIS_SetMF( MFout, Steering, Steering_ZO, gaussmf, NULL, Steering_ZO_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_PS, gaussmf, NULL, Steering_PS_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_NS, gaussmf, NULL, Steering_NS_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_PB, gaussmf, NULL, Steering_PB_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_NB, gaussmf, NULL, Steering_NB_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_PM, gaussmf, NULL, Steering_PM_p, 1.0f );
    qFIS_SetMF( MFout, Steering, Steering_NM, gaussmf, NULL, Steering_NM_p, 1.0f );

    /* Configure the Inference System */
    qFIS_Setup( &Fuzzy, Mamdani,
                Fuzzy_inputs, sizeof(Fuzzy_inputs),
                Fuzzy_outputs, sizeof(Fuzzy_outputs),
                MFin, sizeof(MFin), MFout, sizeof(MFout),
                rules, rStrength, 49u );
    control_p->set_control = set_control_func;

}

uint32_t CONTROL_STEERING_Set_Control(control_steering_t *const control_p, float distance, uint32_t bldc_motor_duty)
{
   switch (bldc_motor_duty)
   {
       case 69:
           control_p->input[1] = 26.8 / VELOCITY_GAIN;
           break;
       case 70:
           control_p->input[1] = 14.4 / VELOCITY_GAIN;
           break;
       case 71:
           control_p->input[1] = 6.8 / VELOCITY_GAIN;
           break;
   }
   control_p->input[0] = distance / DISTANCE_GAIN;
   Fuzzy_run(control_p->input, &(control_p->output));
   control_p->output = control_p->output * DUTY_GAIN + MIDDLE_DUTY;
   return (uint32_t) control_p->output;
}

void Fuzzy_run(float *inputs, float *outputs)
{
    /* Set the crips inputs */
    qFIS_SetInput( Fuzzy_inputs, Angle, inputs[ Angle ] );
    qFIS_SetInput( Fuzzy_inputs, Velocity, inputs[ Velocity ] );

    qFIS_Fuzzify( &Fuzzy );
    if ( qFIS_Inference( &Fuzzy ) > 0 ) {
        qFIS_DeFuzzify( &Fuzzy );
    }
    else {
        /* Error! */
    }

    /* Get the crips outputs */
    outputs[ Steering ] = qFIS_GetOutput( Fuzzy_outputs, Steering );
}

/**
 * The function PID_CONTROL_Create dynamically allocates memory for a control_steering_t structure and
 * initializes it.
 *
 * @return The function `PID_CONTROL_Create()` is returning a pointer to a newly allocated
 * `control_steering_t` structure.
 */
control_steering_t *CONTROL_STEERING_Create()
{
    control_steering_t *control_p = malloc(sizeof(control_steering_t));
    if (control_p != NULL)
    {
        Fuzzy_init(control_p, CONTROL_STEERING_Set_Control);
    }
    return control_p;
}

/**
 * The function PID_CONTROL_Destroy deallocates memory for a PID control structure.
 *
 * @param control_p The `control_p` parameter is a pointer to a structure of type `control_steering_t`.
 */
void CONTROL_STEERING_Destroy(control_steering_t * const control_p)
{
    free(control_p);
}