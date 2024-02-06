/*
 * @file servo_motor.c
 *
 */

#include "servo_motor.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void SERVO_MOTOR_Set_Steering(servo_motor_t *const servo_p)
{
    servo_p->timer_p->Instance->CCR1 = servo_p->duty_steering;
}

void SERVO_MOTOR_Init(servo_motor_t *const servo_p, TIM_HandleTypeDef *timer_p, uint32_t timChannel,
                        void (* set_steering_func)(servo_motor_t *const servo))
{
    servo_p->set_steering = set_steering_func;
    servo_p->timer_p = timer_p;
    HAL_TIM_PWM_Start(servo_p->timer_p, servo_p->timChannel);
    servo_p->duty_steering = DUTY_CYCLE_MIDDLE_STEERING;
    SERVO_MOTOR_Set_Steering(servo_p);
}

servo_motor_t *SERVO_MOTOR_Create(TIM_HandleTypeDef * timer_p, uint32_t timChanel)
{
    servo_motor_t *servo_p = malloc(sizeof(servo_motor_t));
    if (servo_p != NULL)
    {
        SERVO_MOTOR_Init(servo_p, timer_p, timChanel, SERVO_MOTOR_Set_Steering);
    }
    return servo_p;
}

void SERVO_MOTOR_Destroy(servo_motor_t *const servo_p)
{
    free(servo_p);
}
