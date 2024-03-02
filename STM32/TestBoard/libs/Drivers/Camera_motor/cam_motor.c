/*
 * @file CAMERA_MOTOR.c
 *
 */

#include "cam_motor.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function sets the duty cycle of a servo motor's timer instance to control the steering.
 *
 * @param camera_p A pointer to a structure of type CAMERA_MOTOR_t. This structure likely contains
 * information about the servo motor, such as the timer used for PWM control and the desired duty cycle
 * for steering.
 */
void CAMERA_MOTOR_Set_Rotation(camera_motor_t *const camera_p)
{
    switch (camera_p->timChannel)
    {
        case TIM_CHANNEL_1:
            camera_p->timer_p->Instance->CCR1 = camera_p->duty_rotation;
            break;
        case TIM_CHANNEL_2:
        	camera_p->timer_p->Instance->CCR2 = camera_p->duty_rotation;
        	break;
        case TIM_CHANNEL_3:
        	camera_p->timer_p->Instance->CCR3 = camera_p->duty_rotation;
        	break;
        case TIM_CHANNEL_4:
        	camera_p->timer_p->Instance->CCR4 = camera_p->duty_rotation;
        	break;
    }
}

/**
 * The CAMERA_MOTOR_Init function initializes a servo motor by setting its steering function, timer, and
 * starting the PWM signal.
 *
 * @param camera_p A pointer to a CAMERA_MOTOR_t structure, which contains information about the servo
 * motor.
 * @param timer_p A pointer to the TIM_HandleTypeDef structure that represents the timer used for
 * controlling the servo motor.
 * @param timChannel The timChannel parameter is the channel of the timer that is used to control the
 * servo motor. It specifies which channel of the timer is used to generate the PWM signal for the
 * servo motor.
 * @param set_steering_func set_steering_func is a function pointer that points to a function that sets
 * the steering of the servo motor. This function takes a CAMERA_MOTOR_t pointer as a parameter.
 */
void CAMERA_MOTOR_Init(camera_motor_t *const camera_p, TIM_HandleTypeDef *timer_p, uint32_t timChannel,
                      void (*set_rotation_func)(camera_motor_t *const camera_p))
{
    camera_p->set_rotation = set_rotation_func;
    camera_p->timer_p = timer_p;
    camera_p->timChannel = timChannel;
    camera_p->duty_rotation = DUTY_CYCLE_MIDDLE_ROTATION;
    HAL_TIM_PWM_Start(camera_p->timer_p, camera_p->timChannel);
    CAMERA_MOTOR_Set_Rotation(camera_p);
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
 * @return a pointer to a CAMERA_MOTOR_t structure.
 */
camera_motor_t *CAMERA_MOTOR_Create(TIM_HandleTypeDef *timer_p, uint32_t timChanel)
{
    camera_motor_t *camera_p = malloc(sizeof(camera_motor_t));
    if (camera_p != NULL)
    {
        CAMERA_MOTOR_Init(camera_p, timer_p, timChanel, CAMERA_MOTOR_Set_Rotation);
    }
    return camera_p;
}

/**
 * The CAMERA_MOTOR_Destroy function frees the memory allocated for a CAMERA_MOTOR_t structure.
 *
 * @param camera_p The parameter `camera_p` is a pointer to a `CAMERA_MOTOR_t` structure.
 */
void CAMERA_MOTOR_Destroy(camera_motor_t *const camera_p)
{
    free(camera_p);
}
