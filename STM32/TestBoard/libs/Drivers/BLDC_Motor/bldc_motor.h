/*
 * @file bldc_motor.h
 *
 */

#ifndef DRIVERS_BLDC_MOTOR_BLDC_MOTOR_H_
#define DRIVERS_BLDC_MOTOR_BLDC_MOTOR_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

/**********************
 *      TYPEDEFS
 **********************/

struct bldc_motor;
typedef struct bldc_motor bldc_motor_t;

typedef enum
{
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE
} direction_motor_bldc_t;

typedef enum
{
    STOP_SPEED = 0,
    LOW_SPEED,
    HIGH_SPEED
} speed_motor_bldc_t;

struct bldc_motor
{
    direction_motor_bldc_t direction;
    TIM_HandleTypeDef *timer_p;
    speed_motor_bldc_t speed;
    uint32_t timChannel;
    void (*set_speed)(bldc_motor_t *const motor_p);
};

/**********************
 *     OPERATION
 **********************/

bldc_motor_t *BLDC_MOTOR_Create(TIM_HandleTypeDef *timer_p, uint32_t timChannel);
void BLDC_MOTOR_Destroy(bldc_motor_t *const motor_p);

#endif /* DRIVERS_BLDC_MOTOR_BLDC_MOTOR_H_ */