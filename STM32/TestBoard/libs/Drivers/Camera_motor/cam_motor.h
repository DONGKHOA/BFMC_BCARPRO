/*
 * @file servo_motor.h
 *
 */

#ifndef DRIVERS_CAM_SERVO_H_
#define DRIVERS_CAM_SERVO_H_

/*********************
 *      INCLUDES
 *********************/

#include "stdlib.h"
#include "stdint.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define DUTY_CYCLE_MIDDLE_ROTATION 50

/**********************
 *      TYPEDEFS
 **********************/

struct camera_motor;
typedef struct camera_motor camera_motor_t;

struct camera_motor
{
    TIM_HandleTypeDef *timer_p;
    uint32_t timChannel;
    uint32_t duty_rotation;
    void (* set_rotation)(camera_motor_t *const camera_p);
};

/**********************
 *     OPERATION
 **********************/

camera_motor_t *CAMERA_MOTOR_Create(TIM_HandleTypeDef * timer_p, uint32_t timChannel);
void *CAMERA_MOTORO_Destroy(camera_motor_t * const camera_p);

#endif /* DRIVERS_SERVO_MOTOR_SERVO_MOTOR_H_ */
