#ifndef _CONTROL_
#define _CONTROL_

// Include header files for Steering Control
#include "Steering_PID.hpp"
#include "Steering_Setup.hpp"

// Include header files for Speeding Control
#include "Speeding_Setup.hpp"
#include "Speeding_PID.hpp"
#include "IMU.hpp"

namespace drivers
{
    // Steering
    class Steering: public Steering_Setup, public Steering_PID
    {
        private:
            const float Kp_value = 10.0f;
            const float Ki_value = 10.0f;
            const float Kd_value = 10.0f;
            const float PID_sat_value = 10.0f;
            const float Ts = 10.0f;
            /* Variable for process */
            float steering = 0.0f;
        public:
            float Steering_Control(PID_Steering_parameters *PID_Steering_parameters, float distance);
            void Steering_Setup(PID_Steering_parameters *PID_Steering_parameters,
                                const float Kp_value, 
                                const float Ki_value, 
                                const float Kd_value, 
                                const float PID_sat_value, 
                                const float Ts);
    };

    class Speeding: public Speeding_Setup, public Speeding_PID, public Speeding_IMU
    {
        private:
            /* Coefficient of PID */ 
            const float Kp_value = 10.0f;
            const float Ki_value = 10.0f;
            const float Kd_value = 10.0f;
            const float PID_sat_value = 10.0f;
            const float Ts = 10.0f;
            /* Variable for process */
            float PV = 0.0f;
        public:
            float Speeding_Control(PID_Speed_parameters *PID_speed_parameters, float SP, float PV);
            void Speeding_Setup(PID_Speed_parameters *PID_Speed_parameters, 
                                const float Kp_value, 
                                const float Ki_value, 
                                const float Kd_value, 
                                const float PID_sat_value, 
                                const float Ts);
    };
};

#endif