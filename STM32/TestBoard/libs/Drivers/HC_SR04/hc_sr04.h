/*
 * @file hc_sr04.h
 *
 */

#ifndef DRIVERS_HC_SR04_HC_SR04_H_
#define DRIVERS_HC_SR04_HC_SR04_H_

/*********************
 *      INCLUDES
 *********************/

#include "main.h"

/**********************
 *      TYPEDEFS
 **********************/

struct SR04Driver;

typedef struct SR04Driver SR04Driver_t;

struct SR04Driver
{
    TIM_HandleTypeDef *timer_p;
    GPIO_TypeDef *trigPort;
    uint16_t trigPin;
    GPIO_TypeDef *echoPort;
    uint16_t echoPin;

    uint8_t (* get_distance)(SR04Driver_t *const sr04_p);
};

/**********************
 *     OPERATION
 **********************/

SR04Driver_t *SR04_Create(TIM_HandleTypeDef *timer_p, 
                            GPIO_TypeDef *trigPort, uint16_t trigPin,
                            GPIO_TypeDef *echoPort, uint16_t echoPin);

void SR04_Destroy(SR04Driver_t * const sr04_p);

#endif /* DRIVERS_HC_SR04_HC_SR04_H_ */
