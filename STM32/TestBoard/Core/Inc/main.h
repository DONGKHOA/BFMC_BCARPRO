/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_RAS_SCK_Pin GPIO_PIN_5
#define SPI_RAS_SCK_GPIO_Port GPIOA
#define SPI_RAS_MISO_Pin GPIO_PIN_6
#define SPI_RAS_MISO_GPIO_Port GPIOA
#define SPI_RAS_MOSI_Pin GPIO_PIN_7
#define SPI_RAS_MOSI_GPIO_Port GPIOA
#define LED_INIT_BLDC_Pin GPIO_PIN_4
#define LED_INIT_BLDC_GPIO_Port GPIOC
#define ECHO_3_Pin GPIO_PIN_5
#define ECHO_3_GPIO_Port GPIOC
#define MOTOR_BLDC_Pin GPIO_PIN_0
#define MOTOR_BLDC_GPIO_Port GPIOB
#define ECHO_2_Pin GPIO_PIN_2
#define ECHO_2_GPIO_Port GPIOB
#define TRIG_2_Pin GPIO_PIN_12
#define TRIG_2_GPIO_Port GPIOB
#define TRIG_3_Pin GPIO_PIN_6
#define TRIG_3_GPIO_Port GPIOC
#define ECHO_4_Pin GPIO_PIN_8
#define ECHO_4_GPIO_Port GPIOC
#define TRIG_4_Pin GPIO_PIN_9
#define TRIG_4_GPIO_Port GPIOC
#define MOTOR_STEERING_Pin GPIO_PIN_8
#define MOTOR_STEERING_GPIO_Port GPIOA
#define MOTOR_CAM_Pin GPIO_PIN_9
#define MOTOR_CAM_GPIO_Port GPIOA
#define ECHO_1_Pin GPIO_PIN_11
#define ECHO_1_GPIO_Port GPIOA
#define TRIG_1_Pin GPIO_PIN_12
#define TRIG_1_GPIO_Port GPIOA
#define SPI_CS_RAS_Pin GPIO_PIN_6
#define SPI_CS_RAS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
