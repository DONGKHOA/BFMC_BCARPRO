#ifndef _PID_H_
#define _PID_H_

extern int error;

class PID_parameters
{
    public:
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
};

class PID_Controller: public PID_parameters
{
    public:
        // Reset parameters of PID
        void PID_Reset_Parameters(PID_parameters *pid_parameters);
        // Setting PID parameters
        float PID_Set_Parameters(PID_parameters *pid_parameters, float Kp, float Ki, float Kd,     float PID_saturation, float Ts);
        // Processing PID module
        void PID_Process(PID_parameters *pid_parameters, float velocity, float setpoint);
        // Saturation
        float PID_Saturation(PID_parameters *pid_parameters);
        // Read value
        float PID_Read_Value(PID_parameters *pid_parameters);
};

#endif