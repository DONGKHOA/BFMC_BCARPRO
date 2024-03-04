/*
 * @file bldc_motor.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "bldc_motor.h"
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/

// SPEED COUNTER CLOCKWISE

#define DUTY_CYCLE_HIGH_CCK 65
#define DUTY_CYCLE_MEDIUM_CCK 66
#define DUTY_CYCLE_LOW_CCK 68

// SPEED CLOCKWISE

#define DUTY_CYCLE_CK   80

// STOP SPEED

#define DUTY_CYCLE_STOP 75

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function sets the speed of a BLDC motor based on the motor's timer channel, speed, and
 * direction.
 *
 * @param motor_p The parameter `motor_p` is a pointer to a structure of type `bldc_motor_t`.
 */
void BLDC_MOTOR_Set_Speed(bldc_motor_t *const motor_p)
{
    switch (motor_p->timChannel)
    {
    case TIM_CHANNEL_1:

        if (motor_p->speed == STOP_SPEED)
        {
            motor_p->timer_p->Instance->CCR1 = DUTY_CYCLE_STOP;
        }
        else if (motor_p->direction == CLOCKWISE)
        {
            motor_p->timer_p->Instance->CCR1 = DUTY_CYCLE_CK;
        }
        else
        {
            if (motor_p->speed == HIGH_SPEED)
            {
                motor_p->timer_p->Instance->CCR1 = DUTY_CYCLE_HIGH_CCK;
            }
            else if (motor_p->speed == MEDIUM_SPEED)
            {
                motor_p->timer_p->Instance->CCR1 = DUTY_CYCLE_MEDIUM_CCK;
            }
            else
            {
                motor_p->timer_p->Instance->CCR1 = DUTY_CYCLE_LOW_CCK;
            }
            
        }
        
        break;

    case TIM_CHANNEL_2:

        if (motor_p->speed == STOP_SPEED)
        {
            motor_p->timer_p->Instance->CCR2 = DUTY_CYCLE_STOP;
        }
        else if (motor_p->direction == CLOCKWISE)
        {
            motor_p->timer_p->Instance->CCR2 = DUTY_CYCLE_CK;
        }
        else
        {
            if (motor_p->speed == HIGH_SPEED)
            {
                motor_p->timer_p->Instance->CCR2 = DUTY_CYCLE_HIGH_CCK;
            }
            else if (motor_p->speed == MEDIUM_SPEED)
            {
                motor_p->timer_p->Instance->CCR2 = DUTY_CYCLE_MEDIUM_CCK;
            }
            else
            {
                motor_p->timer_p->Instance->CCR2 = DUTY_CYCLE_LOW_CCK;
            }
            
        }
        break;

    case TIM_CHANNEL_3:

        if (motor_p->speed == STOP_SPEED)
        {
            motor_p->timer_p->Instance->CCR3 = DUTY_CYCLE_STOP;
        }
        else if (motor_p->direction == CLOCKWISE)
        {
            motor_p->timer_p->Instance->CCR3 = DUTY_CYCLE_CK;
        }
        else
        {
            if (motor_p->speed == HIGH_SPEED)
            {
                motor_p->timer_p->Instance->CCR3 = DUTY_CYCLE_HIGH_CCK;
            }
            else if (motor_p->speed == MEDIUM_SPEED)
            {
                motor_p->timer_p->Instance->CCR3 = DUTY_CYCLE_MEDIUM_CCK;
            }
            else
            {
                motor_p->timer_p->Instance->CCR3 = DUTY_CYCLE_LOW_CCK;
            }
            
        }
        break;

    case TIM_CHANNEL_4:

        if (motor_p->speed == STOP_SPEED)
        {
            motor_p->timer_p->Instance->CCR4 = DUTY_CYCLE_STOP;
        }
        else if (motor_p->direction == CLOCKWISE)
        {
            motor_p->timer_p->Instance->CCR4 = DUTY_CYCLE_CK;
        }
        else
        {
            if (motor_p->speed == HIGH_SPEED)
            {
                motor_p->timer_p->Instance->CCR4 = DUTY_CYCLE_HIGH_CCK;
            }
            else if (motor_p->speed == MEDIUM_SPEED)
            {
                motor_p->timer_p->Instance->CCR4 = DUTY_CYCLE_MEDIUM_CCK;
            }
            else
            {
                motor_p->timer_p->Instance->CCR4 = DUTY_CYCLE_LOW_CCK;
            }
            
        }
        break;
    }
}

/**
 * The BLDC_MOTOR_Init function initializes a BLDC motor by setting its speed, timer, timer channel,
 * direction, and starting the timer PWM.
 *
 * @param motor_p A pointer to a structure representing the BLDC motor. This structure contains various
 * properties and settings related to the motor.
 * @param timer_p A pointer to the TIM_HandleTypeDef structure that represents the timer used for
 * controlling the motor speed.
 * @param timChannel The `timChannel` parameter is the channel of the timer that will be used for PWM
 * output. It specifies which specific channel of the timer peripheral will be used to generate the PWM
 * signal for controlling the BLDC motor.
 * @param set_speed_func The `set_speed_func` parameter is a function pointer that points to a function
 * which is responsible for setting the speed of the BLDC motor. This function takes a `bldc_motor_t`
 * pointer as a parameter and does not return anything.
 */
void BLDC_MOTOR_Init(bldc_motor_t *const motor_p, TIM_HandleTypeDef *timer_p, uint32_t timChannel,
                     void (*set_speed_func)(bldc_motor_t *const motor))
{
    motor_p->set_speed = set_speed_func;
    motor_p->speed = STOP_SPEED;
    motor_p->timer_p = timer_p;
    motor_p->timChannel = timChannel;
    motor_p->direction = COUNTER_CLOCKWISE;
    HAL_TIM_PWM_Start(motor_p->timer_p, motor_p->timChannel);
    BLDC_MOTOR_Set_Speed(motor_p);
}

/**
 * The function creates and initializes a BLDC motor structure.
 *
 * @param timer_p The parameter "timer_p" is a pointer to a structure of type "TIM_HandleTypeDef". This
 * structure is typically used to configure and control a timer peripheral in a microcontroller. It
 * contains various fields such as the timer's base address, prescaler value, counter mode, etc.
 * @param timChannel The `timChannel` parameter is the channel of the timer peripheral that will be
 * used for controlling the BLDC motor. The specific value of `timChannel` will depend on the timer
 * peripheral being used.
 *
 * @return a pointer to a structure of type bldc_motor_t.
 */
bldc_motor_t *BLDC_MOTOR_Create(TIM_HandleTypeDef *timer_p, uint32_t timChannel)
{
    bldc_motor_t *motor_p = malloc(sizeof(bldc_motor_t));
    if (motor_p != NULL)
    {
        BLDC_MOTOR_Init(motor_p, timer_p, timChannel, BLDC_MOTOR_Set_Speed);
    }
    return motor_p;
}

/**
 * The function `BLDC_MOTOR_Destroy` is used to free the memory allocated for a BLDC motor object.
 *
 * @param motor_p A pointer to a structure representing a BLDC motor.
 */
void BLDC_MOTOR_Destroy(bldc_motor_t *const motor_p)
{
    free(motor_p);
}