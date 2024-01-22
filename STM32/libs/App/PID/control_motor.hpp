/*
 * @file control_motor.hpp
 *
 *  Created on: Dec 1, 2023
 *      Author: dongkhoa
 */

#ifndef APP_PID_PID_HPP_
#define APP_PID_PID_HPP_

#include <iostream>

typedef enum{
    STOP = 0,
    LOW_SPEED,
    HIGH_SPEED
}state_motor_t;

class control_motor
{
    private:
        void CONTROL_MOTOR_Stop(float set_point, float real_speeding);
        void CONTROL_MOTOR_Low_Speed(float set_point, float real_speeding);
        void CONTROL_MOTOR_High_Speed(float set_point, float real_speeding);
    public:
        
};


#endif /* APP_PID_PID_HPP_ */