/*
 * @file hc_sr04.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "delay_timer.h"
#include "hc_sr04.h"
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/

#define MAX_DISTANCE 171600
#define TIME_OUT 2000 // ms

/*********************
 *   STATIC VARIABLE
 *********************/

static uint32_t timer = 0;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function `SR04_Get_Distance` calculates the distance using an ultrasonic sensor and returns the
 * result in centimeters.
 *
 * @param sr04_p The parameter `sr04_p` is a pointer to a structure of type `SR04Driver_t`. This
 * structure contains the following members:
 *
 * @return the distance measured by the SR04 ultrasonic sensor in centimeters.
 */
uint8_t SR04_Get_Distance(SR04Driver_t *const sr04_p)
{
    HAL_GPIO_WritePin(sr04_p->trigPort, sr04_p->trigPin, 1);
    DELAY_Tim_Us(sr04_p->timer_p, 10);

    HAL_TIM_Base_Stop(sr04_p->timer_p);
    HAL_GPIO_WritePin(sr04_p->trigPort, sr04_p->trigPin, 0);

    while (HAL_GPIO_ReadPin(sr04_p->echoPort, sr04_p->echoPin) == 0)
    {
        if ((HAL_GetTick() - timer) > TIME_OUT)
        {
            timer = HAL_GetTick();
            return 0;
        }
    }
    HAL_TIM_Base_Start(sr04_p->timer_p);

    while (1)
    {
        if ((HAL_GetTick() - timer) > TIME_OUT)
        {
            timer = HAL_GetTick();
            return 0;
        }

        if (HAL_GPIO_ReadPin(sr04_p->echoPort, sr04_p->echoPin) == 0)
        {
            HAL_TIM_Base_Stop(sr04_p->timer_p);
            break;
        }

        if ((sr04_p->timer_p->Instance->CNT) > MAX_DISTANCE)
        {
            timer = HAL_GetTick();
            HAL_TIM_Base_Stop(sr04_p->timer_p);
            return 0;
        }
    }
    timer = HAL_GetTick();
    return 0.017 * (sr04_p->timer_p->Instance->CNT); // unit of measure: cm
}

void SR04_Init(SR04Driver_t *sr04_p, TIM_HandleTypeDef *timer_p, GPIO_TypeDef *GPIO_Trig,
               uint16_t GPIO_Pin_Trig, GPIO_TypeDef *GPIO_Echo, uint16_t GPIO_Pin_Echo,
               uint8_t (*get_distance_func)(SR04Driver_t *const sr04_p))
{
    sr04_p->timer_p = timer_p;
    sr04_p->echoPort = GPIO_Echo;
    sr04_p->trigPort = GPIO_Trig;
    sr04_p->echoPin = GPIO_Pin_Echo;
    sr04_p->trigPin = GPIO_Pin_Trig;
    sr04_p->get_distance = get_distance_func;
}

/**********************
 *     OPERATION
 **********************/

SR04Driver_t *SR04_Create(TIM_HandleTypeDef *timer_p,
                          GPIO_TypeDef *trigPort, uint16_t trigPin,
                          GPIO_TypeDef *echoPort, uint16_t echoPin)
{
    SR04Driver_t *sr04_p = malloc(sizeof(SR04Driver_t));

    if (sr04_p != NULL)
    {
        SR04_Init(sr04_p, timer_p, trigPort, trigPin, echoPort, echoPin, SR04_Get_Distance);
    }

    return sr04_p;
}

/**
 * The function SR04_Destroy frees the memory allocated for the SR04Driver_t structure.
 *
 * @param sr04_p The parameter `sr04_p` is a pointer to a `SR04Driver_t` structure.
 */
void SR04_Destroy(SR04Driver_t *const sr04_p)
{
    free(sr04_p);
}