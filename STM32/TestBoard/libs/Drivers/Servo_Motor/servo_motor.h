/*
 * @file servo_motor.h
 *
 */

#ifndef DRIVERS_SERVO_MOTOR_SERVO_MOTOR_H_
#define DRIVERS_SERVO_MOTOR_SERVO_MOTOR_H_

/*********************
 *      INCLUDES
 *********************/

#include "stdlib.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"


/*********************
 *      DEFINES
 *********************/

#define DUTY_CYCLE_MIDDLE_STEERING 50
#define DUTY_CYCLE_MAX_LEFT 25
#define DUTY_CYCLE_MAX_RIGHT 75
#define DUTY_CYCLE_MAX_LEFT         25
#define DUTY_CYCLE_MAX_RIGHT        75

/**********************
 *      TYPEDEFS
 **********************/

struct servo_motor;
typedef struct servo_motor servo_motor_t;

struct servo_motor
{
    TIM_HandleTypeDef *timer_p;
    uint32_t timChannel;
    uint32_t duty_steering;
    void (* set_steering)(servo_motor_t *const servo_p);
};

/**********************
 *     OPERATION
 **********************/

servo_motor_t *SERVO_MOTOR_Create(TIM_HandleTypeDef * timer_p, uint32_t timChannel);
void SERVO_MOTOR_Destroy(servo_motor_t * const servo_p);

#endif /* DRIVERS_SERVO_MOTOR_SERVO_MOTOR_H_ */
