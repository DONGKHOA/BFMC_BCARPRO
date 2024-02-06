/*
 * @file delay_timer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "delay_timer.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function DELAY_Tim_Us is used to introduce a delay in microseconds using a timer.
 * 
 * @param htim The parameter `htim` is a pointer to a `TIM_HandleTypeDef` structure. This structure
 * contains the configuration and state information for a specific timer instance. It is used to
 * control and interact with the timer hardware.
 * @param time_us The parameter "time_us" represents the desired delay time in microseconds.
 */
void DELAY_Tim_Us(TIM_HandleTypeDef *htim, uint16_t time_us)
{
    HAL_TIM_Base_Start(htim);
    htim->Instance->CNT = 0;
    while ((htim->Instance->CNT) < time_us)
        ;
    HAL_TIM_Base_Stop(htim);
}


/**
 * The function DELAY_Tim_Ms is used to introduce a delay in milliseconds using a timer.
 * 
 * @param htim The parameter `htim` is a pointer to a `TIM_HandleTypeDef` structure. This structure is
 * typically used to configure and control a timer peripheral in a microcontroller. It contains various
 * fields such as the timer instance, prescaler value, counter mode, and other settings.
 * @param time_ms The parameter "time_ms" is the delay time in milliseconds. It specifies the amount of
 * time the function should wait before returning.
 */
void DELAY_Tim_Ms(TIM_HandleTypeDef *htim, uint16_t time_ms)
{
    while (time_ms--)
    {
        DELAY_Tim_Us(htim, 1000);
    }
}