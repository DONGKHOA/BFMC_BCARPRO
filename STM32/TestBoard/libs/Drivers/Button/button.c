/*
 * @file button.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "button.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"
#include "timers.h"

/*********************
 *      DEFINES
 *********************/

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

/**********************
 *   WEAK FUNCTIONS
 **********************/

/**
 * The function BUTTON_Pressing_Callback is a weak callback function that takes a pointer to a button
 * structure as a parameter.
 *
 * @param button_p A pointer to a structure representing a button.
 */
__weak void BUTTON_Pressing_Callback(button_t *const button_p)
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
				 void (*handle_func)(button_t *const button_p))
{
	button_p->buttonPort = buttonPort;

	button_p->debouncingTimer = 0;
	button_p->handle = handle_func;
}

void BUTTON_Handle(button_t *const button_p)
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
		BUTTON_Init(button_p, buttonPort, buttonPin, BUTTON_Handle);
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
