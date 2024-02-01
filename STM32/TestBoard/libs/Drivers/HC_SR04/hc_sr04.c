/*
 * @file hc_sr04.c
 *
 */

#include "delay_timer.h"
#include "hc_sr04.h"

#define MAX_DISTANCE 171600
#define TIME_OUT 2000 // ms

static uint32_t timer = 0;

void SR04_Init(SR04_HandleTypeDef *SR_04x, TIM_HandleTypeDef *htim, GPIO_TypeDef *GPIO_Trig, uint16_t GPIO_Pin_Trig, 
                    GPIO_TypeDef *GPIO_Echo, uint16_t GPIO_Pin_Echo)
{
    SR_04x->htim = htim;
    SR_04x->GPIO_Echo = GPIO_Echo;
    SR_04x->GPIO_Trig = GPIO_Trig;
    SR_04x->GPIO_Pin_Echo = GPIO_Pin_Echo;
    SR_04x->GPIO_Pin_Trig = GPIO_Pin_Trig;
}

uint8_t SR04_Get_Distance(SR04_HandleTypeDef *SR_04x)
{

    HAL_GPIO_WritePin(SR_04x->GPIO_Trig, SR_04x->GPIO_Pin_Trig, 1);
    DELAY_Tim_Us(SR_04x->htim, 10);

    HAL_TIM_Base_Stop(SR_04x->htim);
    HAL_GPIO_WritePin(SR_04x->GPIO_Trig, SR_04x->GPIO_Pin_Trig, 0);

    while (HAL_GPIO_ReadPin(SR_04x->GPIO_Echo, SR_04x->GPIO_Pin_Echo) == 0)
    {
        if ((HAL_GetTick() - timer) > TIME_OUT)
        {
        	timer = HAL_GetTick();
            return 0;
        }
    }
    HAL_TIM_Base_Start(SR_04x->htim);

    while (1)
    {
        if ((HAL_GetTick() - timer) > TIME_OUT)
        {
        	timer = HAL_GetTick();
            return 0;
        }

        if (HAL_GPIO_ReadPin(SR_04x->GPIO_Echo, SR_04x->GPIO_Pin_Echo) == 0)
        {
            HAL_TIM_Base_Stop(SR_04x->htim);
            break;
        }

        if ((SR_04x->htim->Instance->CNT) > MAX_DISTANCE)
        {
        	timer = HAL_GetTick();
            HAL_TIM_Base_Stop(SR_04x->htim);
            return 0;
        }
    }
    timer = HAL_GetTick();
    return 0.017 * (SR_04x->htim->Instance->CNT); // unit of measure: cm
}