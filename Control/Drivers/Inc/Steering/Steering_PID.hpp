#ifndef _STEERING_PID_HPP_
#define _STEERING_PID_HPP_

class PID_Steering_parameters
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
class Steering_PID: public PID_Steering_parameters
{
    public:
        void PID_Reset_Parameters(PID_Steering_parameters *PID_Steering_parameters);
        // Setting PID parameters
        float PID_Set_Parameters(PID_Steering_parameters *PID_Steering_parameters, float Kp, float Ki, float Kd, float PID_saturation, float Ts);
        // Processing PID module
        void PID_Process(PID_Steering_parameters *PID_Steering_parameters, float steering);
        // Saturation
        float PID_Saturation(PID_Steering_parameters *PID_Steering_parameters);
        // Read value
        float PID_Read_Value(PID_Steering_parameters *PID_Steering_parameters);
};

#endif