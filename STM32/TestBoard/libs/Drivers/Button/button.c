/*
 * @file button.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "button.h"
#include <stdlib.h>
#include "cmsis_os.h"

/*********************
 *      DEFINES
 *********************/

#define PRESSING_TIMEOUT 2000 // ms

#define BUTTON_Short_Pressing_BIT (1 << 0)
#define BUTTON_Long_Pressing_BIT (1 << 1)
#define BUTTON_Releasing_BIT (1 << 2)

/**********************
 *   STATIC VARIABLE
 **********************/

static uint8_t is_initialize = 0;

static EventGroupHandle_t button_event = NULL;
static TaskHandle_t button_debouncing_task = NULL;
static TaskHandle_t button_executing_task = NULL;
static TimerHandle_t button_time_pressing = NULL;
static QueueHandle_t button_queue_data = NULL;

static button_t *current_button = NULL;

/**********************
 *   WEAK FUNCTIONS
 **********************/

/**
 * The function BUTTON_Pressing_Callback is a weak callback function that takes a pointer to a button
 * structure as a parameter.
 *
 * @param button_p A pointer to a structure representing a button.
 */
__weak void BUTTON_Short_Pressing_Callback(button_t *const button_p)
{
}

/**
 * The function BUTTON_Long_Pressing_Callback is a weak function that takes a pointer to a button
 * structure as a parameter and does not have any implementation.
 *
 * @param button_p A pointer to a structure representing a button.
 */
__weak void BUTTON_Long_Pressing_Callback(button_t *const button_p)
{
}

/**
 * The function BUTTON_Releasing_Callback is a weak function that does not have any implementation.
 *
 * @param button_p A pointer to a structure representing a button.
 */
__weak void BUTTON_Releasing_Callback(button_t *const button_p)
{
}

/*******************************
 *   STATIC PROTOTYPE FUNCTIONS
 *******************************/

static void BUTTON_Debouncing_Task(void *arg);
static void BUTTON_Executing_Task(void *arg);
static void BUTTON_Timer_CallBack_Function();

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * The function initializes a button structure with the given parameters.
 *
 * @param button_p button_p is a pointer to a structure of type button_t. This structure is used to
 * store the state and configuration of a button.
 * @param buttonPort buttonPort is a pointer to the GPIO port that the button is connected to.
 * @param GPIO_Pin_Button GPIO pin number for the button
 * @param handle_func The handle_func parameter is a function pointer that points to a function that
 * will be called when the button is pressed or released. This function takes a pointer to a button_t
 * structure as its argument.
 */
void BUTTON_Init(button_t *const button_p, GPIO_TypeDef *buttonPort, uint16_t GPIO_Pin_Button,
				 void (*exti_handle_func)(button_t *const button_p))
{
	button_p->buttonPort = buttonPort;
	button_p->buttonPin = GPIO_Pin_Button;
	button_p->debouncingTimer = 0;
	button_p->exti_handle = exti_handle_func;
	if (is_initialize == 0)
	{
		button_event = xEventGroupCreate();
		button_queue_data = xQueueCreate(1, sizeof(button_t));

		button_time_pressing = xTimerCreate("button_time_pressing",
											PRESSING_TIMEOUT / portTICK_PERIOD_MS,
											pdFALSE, (void *)0, BUTTON_Timer_CallBack_Function);

		xTaskCreate(BUTTON_Debouncing_Task, "BUTTON_Debouncing_Task",
					128, NULL, osPriorityHigh, &button_debouncing_task);

		xTaskCreate(BUTTON_Executing_Task, "BUTTON_Executing_Task",
					128, NULL, osPriorityAboveNormal, &button_executing_task);
		is_initialize = 1;
	}
}

void BUTTON_Exit_Handle(button_t *const button_p)
{
}

/**********************
 *     OPERATION
 **********************/

/**
 * The function creates a button object and initializes it with the given GPIO port and pin.
 *
 * @param buttonPort buttonPort is a pointer to a GPIO_TypeDef structure, which represents the GPIO
 * port to which the button is connected. GPIO_TypeDef is a structure defined in the STM32 HAL library,
 * and it contains various registers and configuration settings for a GPIO port.
 * @param buttonPin The buttonPin parameter is the pin number on the GPIO port to which the button is
 * connected.
 *
 * @return a pointer to a button_t structure.
 */
button_t *BUTTON_Create(GPIO_TypeDef *buttonPort, uint16_t buttonPin)
{
	button_t *button_p = malloc(sizeof(button_t));
	if (button_p != NULL)
	{
		BUTTON_Init(button_p, buttonPort, buttonPin, BUTTON_Exit_Handle);
	}
	return button_p;
}

/**
 * The BUTTON_Destroy function frees the memory allocated for a button object.
 *
 * @param button_p A pointer to a button object that needs to be destroyed.
 */
void BUTTON_Destroy(button_t *const button_p)
{
	free(button_p);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * The function BUTTON_Debouncing_Task is responsible for debouncing a button press and releasing
 * event.
 *
 * @param arg The "arg" parameter is a void pointer that can be used to pass any additional data or
 * arguments to the BUTTON_Debouncing_Task function. It is not used in the provided code snippet.
 */
static void BUTTON_Debouncing_Task(void *arg)
{
	button_t *button_temp = NULL;
	while (1)
	{
		if (xQueueReceive(button_queue_data, &button_temp, portMAX_DELAY))
		{
			current_button = button_temp;

			if ((HAL_GetTick() - button_temp->debouncingTimer) < 20)
			{
				continue;
			}
			button_temp->debouncingTimer = HAL_GetTick();

			if (HAL_GPIO_ReadPin(button_temp->buttonPort, button_temp->buttonPin) == 0)
			{
				button_temp->pressingTimer = HAL_GetTick();
				xTimerReset(button_time_pressing, 0);
			}
			else
			{
				xTimerStop(button_time_pressing, 0);
				xEventGroupSetBits(button_event, BUTTON_Releasing_BIT);
				if ((HAL_GetTick() - button_temp->pressingTimer) < PRESSING_TIMEOUT)
				{
					xEventGroupSetBits(button_event, BUTTON_Short_Pressing_BIT);
				}
			}
		}
	}
}

/**
 * The function BUTTON_Executing_Task waits for specific bits to be set in the button_event event group
 * and calls the corresponding callback functions based on the set bits.
 *
 * @param arg The "arg" parameter is a void pointer that can be used to pass any additional arguments
 * or data to the BUTTON_Executing_Task function. It can be cast to the appropriate data type inside
 * the function to access the passed data.
 */
static void BUTTON_Executing_Task(void *arg)
{
	while (1)
	{
		EventBits_t uxBits = xEventGroupWaitBits(button_event,
												 BUTTON_Short_Pressing_BIT | BUTTON_Releasing_BIT,
												 pdTRUE, pdFALSE, portMAX_DELAY);

		if (uxBits & BUTTON_Short_Pressing_BIT)
		{
			BUTTON_Short_Pressing_Callback(current_button);
		}
		else if (uxBits & BUTTON_Releasing_BIT)
		{
			BUTTON_Releasing_Callback(current_button);
		}
	}
}

/**
 * The function BUTTON_Timer_CallBack_Function is a static function that calls the
 * BUTTON_Long_Pressing_Callback function and stops a timer.
 */
static void BUTTON_Timer_CallBack_Function()
{
	BUTTON_Long_Pressing_Callback(current_button);
	xTimerStop(button_time_pressing, 0);
}
