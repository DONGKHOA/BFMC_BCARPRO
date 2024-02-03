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
    uint16_t buttonPin;
    uint8_t currentState;
    uint8_t lastState;
    uint8_t debouncingState;
    uint8_t isDebouncing;
    uint32_t debouncingTimer;
    GPIO_TypeDef *buttonPort;

    void (* handle)(button * const button_p)
};

/**********************
 *     OPERATION
 **********************/

button_t *BUTTON_Create()
#endif /* DRIVERS_BUTTON_BUTTON_H_ */