/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bldc_motor.h"
#include "hc_sr04.h"
#include "servo_motor.h"
#include "ps2.h"
#include "mpu9250_app.h"
#include "debug.h"
#include "situation.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

osThreadId getDataIMUHandle;
osThreadId situationHandle;
osThreadId controlSteeringHandle;
osThreadId ps2ControlHandle;
osThreadId receiveDataSpiHandle;
osThreadId getDistance_SR0Handle;
/* USER CODE BEGIN PV */

// Queue send data
static QueueHandle_t queue_data = NULL;

// EventGroup
static EventGroupHandle_t controlEvent;

// Semaphore enable receive data
static SemaphoreHandle_t receiveSemaphore;

// BLDC motor driver

static bldc_motor_t *bldc_motor_0;
// Servo Motor driver

static servo_motor_t *steering_motor_p;

// SR04 measure distance from car to barrier

static SR04Driver_t *sr04_0;
static SR04Driver_t *sr04_1;
static SR04Driver_t *sr04_2;
static SR04Driver_t *sr04_3;

static sr04_state_t state_sr04;

static servo_motor_t *camera_motor_p;

// imu9250

static imu_9250_t *imu_9250_p;
uint16_t error;
static Struct_Angle Angle;
uint32_t temp_systick = 0;
static float yaw_angle_init = 0;
// Variable of SPI

// data is receive from spi communication
uint8_t data_receiv;

// array store data after receiving from spi communication
static uint8_t spi_buff[5];

// index in array, position of data
uint8_t spi_datapos = 0;

// flag notify complete receive data
static uint8_t spi_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_SPI3_Init(void);
void StartgetDataIMU(void const * argument);
void StartSituation(void const * argument);
void StartControlSteering(void const * argument);
void startps2Control(void const * argument);
void StartreceiveDataSpi(void const * argument);
void StartgetDistance_SR04(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern void calibrateGyro(imu_9250_t *mpu9250, uint16_t numCalPoints);

// Interrupt function of spi (RX)

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t xhigherprioritytaskwoken = pdFALSE;
    if (hspi->Instance == hspi1.Instance)
    {
        HAL_SPI_Receive_IT(&hspi1, &data_receiv, 1);

		spi_buff[(spi_datapos++)] = data_receiv;
		if (spi_datapos == 5)
		{
			spi_datapos = 0;
			spi_flag = 1;
		}
//      xTaskNotifyFromISR(receiveDataSpiHandle, 0, eNotifyAction, &xhigherprioritytaskwoken);
    }
    if(xhigherprioritytaskwoken)
    {
    	taskYIELD();
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /*if (GPIO_Pin == GPIO_PIN_1)
    {
        if (((GPIOC->IDR) & (0b00000010)) == 0)
        else
        {
            spi_flag++;
        }
        spi_datapos = 0;
    }*/
}
static inline void SPI_Handle_Data(void)
{
//  if (spi_flag == 1)
//  {
//    // send queue
//    xQueueSend(queue_data, (void *)&rx_buff, 0);
//    spi_flag = 0;
//  }

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */

  // Initialization driver
  bldc_motor_0 = BLDC_MOTOR_Create(&htim3, TIM_CHANNEL_3);
  steering_motor_p = SERVO_MOTOR_Create(&htim1, TIM_CHANNEL_1);
  HAL_Delay(2000);
  bldc_motor_0->speed = MEDIUM_SPEED;
  bldc_motor_0->direction = COUNTER_CLOCKWISE;
  bldc_motor_0->set_speed(bldc_motor_0);

  controlEvent = xEventGroupCreate();
  queue_data = xQueueCreate(5, sizeof(uint8_t));
  receiveSemaphore = xSemaphoreCreateBinary();

  camera_motor_p = SERVO_MOTOR_Create(&htim1, TIM_CHANNEL_2);

  sr04_0 = SR04_Create(&htim4, TRIG_1_GPIO_Port, TRIG_1_Pin, ECHO_1_GPIO_Port, ECHO_1_Pin);
  sr04_1 = SR04_Create(&htim4, TRIG_2_GPIO_Port, TRIG_2_Pin, ECHO_2_GPIO_Port, ECHO_2_Pin);
  sr04_2 = SR04_Create(&htim4, TRIG_3_GPIO_Port, TRIG_3_Pin, ECHO_3_GPIO_Port, ECHO_3_Pin);
  sr04_3 = SR04_Create(&htim4, TRIG_4_GPIO_Port, TRIG_4_Pin, ECHO_4_GPIO_Port, ECHO_4_Pin);
  imu_9250_p = IMU_9250_Create();
  calibrateGyro(imu_9250_p, 999);

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of getDataIMU */
  osThreadDef(getDataIMU, StartgetDataIMU, osPriorityHigh, 0, 128);
  getDataIMUHandle = osThreadCreate(osThread(getDataIMU), NULL);

  /* definition and creation of situation */
  osThreadDef(situation, StartSituation, osPriorityAboveNormal, 0, 128);
  situationHandle = osThreadCreate(osThread(situation), NULL);

  /* definition and creation of controlSteering */
  osThreadDef(controlSteering, StartControlSteering, osPriorityBelowNormal, 0, 128);
  controlSteeringHandle = osThreadCreate(osThread(controlSteering), NULL);

  /* definition and creation of ps2Control */
  osThreadDef(ps2Control, startps2Control, osPriorityHigh, 0, 128);
  ps2ControlHandle = osThreadCreate(osThread(ps2Control), NULL);

  /* definition and creation of receiveDataSpi */
  osThreadDef(receiveDataSpi, StartreceiveDataSpi, osPriorityHigh, 0, 128);
  receiveDataSpiHandle = osThreadCreate(osThread(receiveDataSpi), NULL);

  /* definition and creation of getDistance_SR0 */
  osThreadDef(getDistance_SR0, StartgetDistance_SR04, osPriorityRealtime, 0, 128);
  getDistance_SR0Handle = osThreadCreate(osThread(getDistance_SR0), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 448-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 112-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 70-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_INIT_BLDC_Pin|TRIG_3_Pin|TRIG_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TRIG_2_Pin|CONTROL_RAS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TRIG_1_GPIO_Port, TRIG_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_INIT_BLDC_Pin TRIG_3_Pin TRIG_4_Pin */
  GPIO_InitStruct.Pin = LED_INIT_BLDC_Pin|TRIG_3_Pin|TRIG_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ECHO_3_Pin ECHO_4_Pin */
  GPIO_InitStruct.Pin = ECHO_3_Pin|ECHO_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ECHO_2_Pin */
  GPIO_InitStruct.Pin = ECHO_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ECHO_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TRIG_2_Pin CONTROL_RAS_Pin */
  GPIO_InitStruct.Pin = TRIG_2_Pin|CONTROL_RAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ECHO_1_Pin */
  GPIO_InitStruct.Pin = ECHO_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ECHO_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TRIG_1_Pin */
  GPIO_InitStruct.Pin = TRIG_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TRIG_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI_CS_RAS_Pin */
  GPIO_InitStruct.Pin = SPI_CS_RAS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SPI_CS_RAS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartgetDataIMU */
/**
 * @brief  Function implementing the getDataIMU thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartgetDataIMU */
void StartgetDataIMU(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for (;;)
  {
    EventBits_t uxBits = xEventGroupWaitBits(controlEvent,
                                             BARRIER_AVOID,
                                             pdFALSE, pdFALSE, portMAX_DELAY);
    if (uxBits & BARRIER_AVOID)
    {
      imu_9250_p->get_data(imu_9250_p);
      CalculateGyroAngle(&Angle, imu_9250_p);
      if (yaw_angle_init - Angle.gyro_yaw >= LANE_CHANGE_ANGLE)
      {
        xEventGroupSetBits(controlEvent, STEERING_NONE_CONTROL_BIT);
        steering_motor_p->duty_steering = DUTY_CYCLE_MAX_RIGHT;
      }
      else if ((yaw_angle_init - Angle.gyro_yaw < STEERING_CHANGE_ANGLE) &&
               (yaw_angle_init - Angle.gyro_yaw > -STEERING_CHANGE_ANGLE))
      {
        state_sr04.state_right = 1;
        vTaskSuspend(getDataIMUHandle);
      }
      else if (yaw_angle_init - Angle.gyro_yaw >= -LANE_CHANGE_ANGLE)
      {
        xEventGroupSetBits(controlEvent, STEERING_NONE_CONTROL_BIT);
        steering_motor_p->duty_steering = DUTY_CYCLE_MAX_LEFT;
        vTaskSuspend(getDataIMUHandle);
      }

      if ((HAL_GetTick() - temp_systick) == 3000)
      {
        temp_systick = 0;
        MPU9250_Writebyte(MPU9250_PWR_MGMT_1, 0x1 << 7);
        osDelay(100);
        MPU9250_Writebyte(MPU9250_PWR_MGMT_1, 0x00);
        osDelay(50);
      }
    }

    osDelay(10);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartSituation */
/**
 * @brief Function implementing the situation thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSituation */
void StartSituation(void const * argument)
{
  /* USER CODE BEGIN StartSituation */
  uint8_t buffer[5];
  uint8_t condition;
  /* Infinite loop */
  for (;;)
  {
    if (xQueueReceive(queue_data, buffer, portMAX_DELAY))
    {
      switch (buffer[0])
      {
      case DISTANCE_LANE:

        xEventGroupSetBits(controlEvent, SPEEDING_BIT | STEERING_CONTROL_BIT);
        break;
      case TRAFFIC_SIGNS:
        if (buffer[1] == TRAFFIC_SIGN_CROSSWALK)
        {
          condition = 0;
        }
        else
        {
          condition = 1;
        }
        xEventGroupSetBits(controlEvent, SPEEDING_BIT | STEERING_CONTROL_BIT);
        break;
      case TRAFFIC_LIGHTS:
        xEventGroupSetBits(controlEvent, SPEEDING_BIT);
        break;
      case INDEFINITE_LANE:
        xEventGroupSetBits(controlEvent, SPEEDING_BIT | CAMERA_BIT);
        break;
      case BARRIER:
        if (condition == 1)
        {
          steering_motor_p->duty_steering = DUTY_CYCLE_MAX_LEFT;
          CalculateGyroAngle(&Angle, imu_9250_p);
          yaw_angle_init = Angle.gyro_yaw;
          vTaskResume(getDataIMUHandle);
          xEventGroupSetBits(controlEvent, SPEEDING_BIT | STEERING_NONE_CONTROL_BIT | BARRIER_AVOID);
          vTaskSuspend(situationHandle);
        }

        break;
      default:
        break;
      }
    }

    osDelay(1);
  }
  /* USER CODE END StartSituation */
}

/* USER CODE BEGIN Header_StartControlSteering */
/**
 * @brief Function implementing the controlSteering thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartControlSteering */
void StartControlSteering(void const * argument)
{
  /* USER CODE BEGIN StartControlSteering */
  /* Infinite loop */
  for (;;)
  {
    EventBits_t uxBits = xEventGroupWaitBits(controlEvent,
                                             STEERING_CONTROL_BIT | STEERING_NONE_CONTROL_BIT,
                                             pdTRUE, pdFALSE, portMAX_DELAY);

    if (uxBits & STEERING_CONTROL_BIT)
    {
      // Control Steering
    }

    if (uxBits & STEERING_NONE_CONTROL_BIT)
    {
      steering_motor_p->set_steering(steering_motor_p);
    }

    osDelay(1);
  }
  /* USER CODE END StartControlSteering */
}

/* USER CODE BEGIN Header_startps2Control */
/**
 * @brief Function implementing the ps2Control thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startps2Control */
void startps2Control(void const * argument)
{
  /* USER CODE BEGIN startps2Control */
#if DEBUG_USER
  vTaskDelete(receiveDataSpiHandle);
  vTaskDelete(situationHandle);
  vTaskDelete(controlSteeringHandle);
  vTaskDelete(getDistance_SR0Handle);
  vTaskDelete(getDataIMUHandle);
  HAL_SPI_DeInit(&hspi1);
#else
  
  HAL_SPI_DeInit(&hspi3);
//  HAL_SPI_Receive_IT(&hspi1, &data_receiv, 1);
  vTaskDelete(ps2ControlHandle);
#endif
  /* Infinite loop */
  for (;;)
  {
    PS2_Data(GPIOD, GPIO_PIN_2, &hspi3, steering_motor_p, bldc_motor_0);
    osDelay(10);
  }
  /* USER CODE END startps2Control */
}

/* USER CODE BEGIN Header_StartreceiveDataSpi */
/**
 * @brief Function implementing the receiveDataSpi thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartreceiveDataSpi */
void StartreceiveDataSpi(void const * argument)
{
  /* USER CODE BEGIN StartreceiveDataSpi */
  /* Infinite loop */
  for (;;)
  {
    if (xSemaphoreTake(receiveSemaphore, portMAX_DELAY))
    {
//    	xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
      SPI_Handle_Data();
    }

    osDelay(1);
  }
  /* USER CODE END StartreceiveDataSpi */
}

/* USER CODE BEGIN Header_StartgetDistance_SR04 */
/**
 * @brief Function implementing the getDistance_SR0 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartgetDistance_SR04 */
void StartgetDistance_SR04(void const * argument)
{
  /* USER CODE BEGIN StartgetDistance_SR04 */
  /* Infinite loop */
  for (;;)
  {
    // 0: forward
    // 1: right
    // 2: back
    // 3: left

    // sr04_1->get_distance(sr04_1);
    // sr04_2->get_distance(sr04_2);
    // sr04_3->get_distance(sr04_3);
      HAL_SPI_Receive_IT(&hspi1, &data_receiv, 1);

//	  HAL_SPI_Receive_DMA(&hspi1, &data_receiv, 1);
//	  spi_buff[ii++] = data_receiv;
    if (sr04_0->get_distance(sr04_0) > DISTANCE_BARRIER)
    {
      HAL_GPIO_WritePin(CONTROL_RAS_GPIO_Port, CONTROL_RAS_Pin, 1);
      __HAL_SPI_ENABLE_IT(&hspi1, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
      xSemaphoreGive(receiveSemaphore);
    }
    else
    {
      HAL_GPIO_WritePin(CONTROL_RAS_GPIO_Port, CONTROL_RAS_Pin, 0);
//      __HAL_SPI_DISABLE_IT(&hspi1, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
    }

    if (state_sr04.state_right == 1)
    {
      if (sr04_1->get_distance(sr04_1) > DISTANCE_BESIDE_BARRIER)
      {

        state_sr04.state_right = 0;
        HAL_GPIO_WritePin(CONTROL_RAS_GPIO_Port, CONTROL_RAS_Pin, 0);
//        __HAL_SPI_DISABLE_IT(&hspi1, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
        CalculateGyroAngle(&Angle, imu_9250_p);
        yaw_angle_init = Angle.gyro_yaw;
        steering_motor_p->duty_steering = DUTY_CYCLE_MAX_RIGHT;
        xEventGroupSetBits(controlEvent, STEERING_NONE_CONTROL_BIT);
        osDelay(10);
        vTaskResume(getDataIMUHandle);
      }
      else
      {
        HAL_GPIO_WritePin(CONTROL_RAS_GPIO_Port, CONTROL_RAS_Pin, 1);
//        __HAL_SPI_ENABLE_IT(&hspi1, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
        xSemaphoreGive(receiveSemaphore);
      }
    }

    osDelay(1);
  }
  /* USER CODE END StartgetDistance_SR04 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
