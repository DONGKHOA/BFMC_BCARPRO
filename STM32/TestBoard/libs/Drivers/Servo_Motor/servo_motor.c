/*
 * @file servo_motor.c
 *
 */

#include "servo_motor.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function sets the duty cycle of a servo motor's timer instance to control the steering.
 * 
 * @param servo_p A pointer to a structure of type servo_motor_t. This structure likely contains
 * information about the servo motor, such as the timer used for PWM control and the desired duty cycle
 * for steering.
 */
void SERVO_MOTOR_Set_Steering(servo_motor_t *const servo_p)
{
    servo_p->timer_p->Instance->CCR1 = servo_p->duty_steering;
}

/**
 * The SERVO_MOTOR_Init function initializes a servo motor by setting its steering function, timer, and
 * starting the PWM signal.
 * 
 * @param servo_p A pointer to a servo_motor_t structure, which contains information about the servo
 * motor.
 * @param timer_p A pointer to the TIM_HandleTypeDef structure that represents the timer used for
 * controlling the servo motor.
 * @param timChannel The timChannel parameter is the channel of the timer that is used to control the
 * servo motor. It specifies which channel of the timer is used to generate the PWM signal for the
 * servo motor.
 * @param set_steering_func set_steering_func is a function pointer that points to a function that sets
 * the steering of the servo motor. This function takes a servo_motor_t pointer as a parameter.
 */
void SERVO_MOTOR_Init(servo_motor_t *const servo_p, TIM_HandleTypeDef *timer_p, uint32_t timChannel,
                        void (* set_steering_func)(servo_motor_t *const servo_p))
{
    servo_p->set_steering = set_steering_func;
    servo_p->timer_p = timer_p;
    HAL_TIM_PWM_Start(servo_p->timer_p, servo_p->timChannel);
    servo_p->duty_steering = DUTY_CYCLE_MIDDLE_STEERING;
    SERVO_MOTOR_Set_Steering(servo_p);
}

/**
 * The function creates a servo motor object and initializes it with the given timer and channel.
 * 
 * @param timer_p The parameter "timer_p" is a pointer to a structure of type "TIM_HandleTypeDef". This
 * structure is typically used to configure and control a timer peripheral in a microcontroller. It
 * contains various fields such as the timer's base address, prescaler value, counter mode, etc.
 * @param timChanel The "timChanel" parameter is the channel of the timer peripheral that will be used
 * to control the servo motor. It is typically represented by a number, such as 1, 2, 3, etc.,
 * depending on the specific microcontroller or development board you are using. The channel
 * 
 * @return a pointer to a servo_motor_t structure.
 */
servo_motor_t *SERVO_MOTOR_Create(TIM_HandleTypeDef * timer_p, uint32_t timChanel)
{
    servo_motor_t *servo_p = malloc(sizeof(servo_motor_t));
    if (servo_p != NULL)
    {
        SERVO_MOTOR_Init(servo_p, timer_p, timChanel, SERVO_MOTOR_Set_Steering);
    }
    return servo_p;
}

/**
 * The SERVO_MOTOR_Destroy function frees the memory allocated for a servo_motor_t structure.
 * 
 * @param servo_p The parameter `servo_p` is a pointer to a `servo_motor_t` structure.
 */
void SERVO_MOTOR_Destroy(servo_motor_t *const servo_p)
{
    free(servo_p);
}
