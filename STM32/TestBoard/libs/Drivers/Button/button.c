/*
 * @file button.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "button.h"
#include <stdlib.h>

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
void BUTTON_Init(button_t * const button_p, GPIO_TypeDef *buttonPort, uint16_t GPIO_Pin_Button,
					void (* handle_func)(button_t * const button_p))
{
	button_p->buttonPort = buttonPort;
	button_p->currentState = 1;
	button_p->lastState = 1;
	button_p->debouncingState = 1;
	button_p->isDebouncing = 0;
	button_p->debouncingTimer = 0;
	button_p->handle = handle_func;
}


/**
 * The function BUTTON_Handle handles button press and release events, including debouncing the button
 * signal.
 * 
 * @param button_p A pointer to a structure that represents a button. The structure contains the
 * following members:
 */
void BUTTON_Handle(button_t * const button_p)
{
	// Detecting
	uint8_t tempState = HAL_GPIO_ReadPin(button_p->buttonPort, button_p->buttonPin);
	if (tempState != button_p->debouncingState)
	{
		button_p->debouncingState = tempState;
		button_p->debouncingTimer = HAL_GetTick();
		button_p->isDebouncing = 1;
	}

	// Debouncing

	if ((button_p->isDebouncing == 1) && (HAL_GetTick() - button_p->debouncingTimer > 15))
	{
		button_p->currentState = button_p->debouncingState;
		button_p->isDebouncing = 0;
	}
	
	if (button_p->currentState != button_p->lastState)
	{
		if (button_p->currentState == 0)	
		{
			//Press button
			BUTTON_Pressing_Callback(button_p);
		}
		else
		{
			// Release button
			BUTTON_Releasing_Callback(button_p);
		}
	}
	
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
button_t * BUTTON_Create(GPIO_TypeDef *buttonPort, uint16_t buttonPin)
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