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
    uint8_t is_pressing;
    uint16_t buttonPin;
    uint32_t debouncingTimer;
    uint32_t pressingTimer;
    GPIO_TypeDef *buttonPort;

    void (* exti_handle)(button_t * const button_p);
};

/**********************
 *     OPERATION
 **********************/

button_t * BUTTON_Create(GPIO_TypeDef *buttonPort, uint16_t buttonPin);
void BUTTON_Destroy(button_t *const button_p);

#endif /* DRIVERS_BUTTON_BUTTON_H_ */