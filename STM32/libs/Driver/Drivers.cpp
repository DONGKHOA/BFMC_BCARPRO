#include "Drivers.hpp"
#include "Steering_PID.hpp"
#include "Distance_Handle.hpp"
#include "Speeding_PID.hpp"
#include <iostream>

Speeding_PID_Setup *Speeding_PID_Setup::instance = nullptr;
Speeding_PID_Control *Speeding_PID_Control::instance = nullptr;
Steering_PID_Setup *Steering_PID_Setup::instance = nullptr;
Steering_PID_Control *Steering_PID_Control::instance = nullptr;

void Control_Setup()
{
    Speeding_PID_Setup *Speeding_PID_Setup_instance = Speeding_PID_Setup::getInstance();
    Speeding_PID_Setup_instance->Speeding_Setup();
    Steering_PID_Setup *Steering_PID_Setup_instance = Steering_PID_Setup::getInstance();
    Steering_PID_Setup::getInstance();
    Steering_PID_Setup_instance->Steering_Setup();
}

void Speeding(float SP, float PV)
{
    Speeding_PID_Control *Speeding_PID_Control_instance = Speeding_PID_Control::getInstance();
    Speeding_PID_Control_instance->Control_Speeding(SP, PV);
}

void Steering(float distance)
{
    Steering_PID_Control *Steering_PID_Control_instance = Steering_PID_Control::getInstance();
    Steering_PID_Control_instance->Control_Steering(distance);
}