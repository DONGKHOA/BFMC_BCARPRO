/*
 * @file fuzzy_control.h
 *
 */

#ifndef MIDDLEWARES_FUZZY_CONTROL_FUZZY_CONTROL_H_
#define MIDDLEWARES_FUZZY_CONTROL_FUZZY_CONTROL_H_

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float error;
    float pre_error;
    float pre2_error;
    float Ts;
    float Kp_part;
    float Ki_part;
    float Kd_part;
    float Out;
    float pre_Out;
    float PID_saturation;
    float steering;
} PID_Steering_parameters;

/* Controller Setup */
// Reset the parameters
void PID_Reset_Parameters(PID_Steering_parameters *PID_para);
// Setting PID parameters
void PID_Set_Parameters(PID_Steering_parameters *PID_para);

/* Controller Processing */
// Processing PID module
void PID_Process(PID_Steering_parameters *PID_para, float steering);
// Saturation
float PID_Saturation(PID_Steering_parameters *PID_para);
/* Controller Output */
float PID_Read_Output(PID_Steering_parameters *PID_para);

#endif /* MIDDLEWARES_FUZZY_CONTROL_FUZZY_CONTROL_H_ */
