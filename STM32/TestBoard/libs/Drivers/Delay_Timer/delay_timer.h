/*
 * @file delay_timer.h
 *
 */

#ifndef DRIVERS_DELAY_TIMER_DELAY_TIMER_H_
#define DRIVERS_DELAY_TIMER_DELAY_TIMER_H_
/*********************
 *      INCLUDES
 *********************/

#include "main.h"

void DELAY_Tim_Us(TIM_HandleTypeDef *htim, uint16_t time_us);

void DELAY_Tim_Ms(TIM_HandleTypeDef *htim, uint16_t time_ms);

#endif /* DRIVERS_DELAY_TIMER_DELAY_TIMER_H_ */
