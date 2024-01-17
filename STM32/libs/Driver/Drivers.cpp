#include "Drivers.hpp"
#include "Steering_PID.hpp"
#include "Distance_Handle.hpp"
#include "Speeding_PID.hpp"
#include <iostream>

Speeding_PID_Setup *Speeding_PID_Setup::instance = nullptr;
Speeding_PID_Control *Speeding_PID_Control::instance = nullptr;
Steering_PID_Setup *Steering_PID_Setup::instance = nullptr;
Steering_PID_Control *Steering_PID_Control::instance = nullptr;

/**
 * The function Control_Setup sets up the Speeding PID and Steering PID instances and calls their
 * respective setup functions.
 */
void Control_Setup()
{
    Speeding_PID_Setup *Speeding_PID_Setup_instance = Speeding_PID_Setup::getInstance();
    Speeding_PID_Setup_instance->Speeding_Setup();
    Steering_PID_Setup *Steering_PID_Setup_instance = Steering_PID_Setup::getInstance();
    Steering_PID_Setup::getInstance();
    Steering_PID_Setup_instance->Steering_Setup();
}

/**
 * The function Speeding takes in two float parameters, SP and PV, and calls the Control_Speeding
 * function of the Speeding_PID_Control class instance to control speeding.
 * 
 * @param SP SP stands for Set Point, which is the desired speed that the system should maintain.
 * @param PV PV stands for Process Variable. In the context of this code snippet, it represents the
 * current speed of a vehicle or any other process that is being controlled.
 */
void Speeding(float SP, float PV)
{
    Speeding_PID_Control *Speeding_PID_Control_instance = Speeding_PID_Control::getInstance();
    Speeding_PID_Control_instance->Control_Speeding(SP, PV);
}

/**
 * The function "Steering" controls the steering of a vehicle using a PID controller.
 * 
 * @param distance The distance parameter represents the desired distance for the steering control. It
 * is passed to the Control_Steering function of the Steering_PID_Control class.
 */
void Steering(float distance)
{
    Steering_PID_Control *Steering_PID_Control_instance = Steering_PID_Control::getInstance();
    Steering_PID_Control_instance->Control_Steering(distance);
}