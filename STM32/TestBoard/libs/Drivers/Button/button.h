/*
 * @file button.h
 *
 */

#ifndef DRIVERS_BUTTON_BUTTON_H_
#define DRIVERS_BUTTON_BUTTON_H_

/*********************
 *      INCLUDES
 *********************/

#include "main.h"

/**********************
 *      TYPEDEFS
 **********************/

struct button;
typedef struct button button_t;

struct button
{
    uint8_t currentState : 1;
    uint8_t lastState : 1;
    uint8_t debouncingState : 1;
    uint8_t isDebouncing : 1;
    uint16_t buttonPin;
    uint32_t debouncingTimer;
    GPIO_TypeDef *buttonPort;

    void (* handle)(button_t * const button_p);
};

/**********************
 *     OPERATION
 **********************/

button_t * BUTTON_Create(GPIO_TypeDef *buttonPort, uint16_t buttonPin);
void BUTTON_Destroy(button_t *const button_p);

#endif /* DRIVERS_BUTTON_BUTTON_H_ */