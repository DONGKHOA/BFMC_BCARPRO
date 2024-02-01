/*
 * @file bldc_motor.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "bldc_motor.h"
#include <stdlib.h>

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void BLDC_MOTOR_Set_Speed(bldc_motor_t *const motor)
{
    switch (motor->timChannel)
    {
    case TIM_CHANNEL_1:
        if (motor->speed == STOP_SPEED)
        {
            motor->timer_p->Instance->CCR1 = 75;
        }
        else
        {
            if (motor->direction == COUNTER_CLOCKWISE)
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR1 = DUTY_CYCLE_LOW_SPEED_CCW;
                }
                else
                {
                    motor->timer_p->Instance->CCR1 = DUTY_CYCLE_HIGH_SPEED_CCW;
                }
            }
            else
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR1 = DUTY_CYCLE_LOW_SPEED_CW;
                }
                else
                {
                    motor->timer_p->Instance->CCR1 = DUTY_CYCLE_HIGH_SPEED_CW;
                }
            }
        }

        break;

    case TIM_CHANNEL_2:

        if (motor->speed == STOP_SPEED)
        {
            motor->timer_p->Instance->CCR2 = 75;
        }
        else
        {
            if (motor->direction == COUNTER_CLOCKWISE)
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR2 = DUTY_CYCLE_LOW_SPEED_CCW;
                }
                else
                {
                    motor->timer_p->Instance->CCR2 = DUTY_CYCLE_HIGH_SPEED_CCW;
                }
            }
            else
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR2 = DUTY_CYCLE_LOW_SPEED_CW;
                }
                else
                {
                    motor->timer_p->Instance->CCR2 = DUTY_CYCLE_HIGH_SPEED_CW;
                }
            }
        }
        break;

    case TIM_CHANNEL_3:

        if (motor->speed == STOP_SPEED)
        {
            motor->timer_p->Instance->CCR3 = 75;
        }
        else
        {
            if (motor->direction == COUNTER_CLOCKWISE)
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR3 = DUTY_CYCLE_LOW_SPEED_CCW;
                }
                else
                {
                    motor->timer_p->Instance->CCR3 = DUTY_CYCLE_HIGH_SPEED_CCW;
                }
            }
            else
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR3 = DUTY_CYCLE_LOW_SPEED_CW;
                }
                else
                {
                    motor->timer_p->Instance->CCR3 = DUTY_CYCLE_HIGH_SPEED_CW;
                }
            }
        }
        break;

    case TIM_CHANNEL_4:

        if (motor->speed == STOP_SPEED)
        {
            motor->timer_p->Instance->CCR4 = 75;
        }
        else
        {
            if (motor->direction == COUNTER_CLOCKWISE)
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR4 = DUTY_CYCLE_LOW_SPEED_CCW;
                }
                else
                {
                    motor->timer_p->Instance->CCR4 = DUTY_CYCLE_HIGH_SPEED_CCW;
                }
            }
            else
            {
                if (motor->speed == LOW_SPEED)
                {
                    motor->timer_p->Instance->CCR4 = DUTY_CYCLE_LOW_SPEED_CW;
                }
                else
                {
                    motor->timer_p->Instance->CCR4 = DUTY_CYCLE_HIGH_SPEED_CW;
                }
            }
        }
        break;
    }
    
}

void BLDC_MOTOR_Init(bldc_motor_t * const motor, TIM_HandleTypeDef *timer_p, uint32_t timChannel,
                        void (* set_speed_func)(bldc_motor_t *const motor))
{
    motor->set_speed = set_speed_func;
    motor->speed = STOP_SPEED;
    motor->timer_p = timer_p;
    motor->timChannel = timChannel;
    motor->direction = COUNTER_CLOCKWISE;
    HAL_TIM_PWM_Start(motor->timer_p, motor->timChannel);
    BLDC_MOTOR_Set_Speed(motor);
}

bldc_motor_t *BLDC_MOTOR_Create(TIM_HandleTypeDef * timer_p, uint32_t timChannel)
{
    bldc_motor_t *motor = malloc(sizeof(bldc_motor_t));
    if (motor != NULL)
    {
        BLDC_MOTOR_Init(motor, timer_p, timChannel, BLDC_MOTOR_Set_Speed);
    }
    return motor;
}

void BLDC_MOTOR_Destroy(bldc_motor_t * const motor)
{

}