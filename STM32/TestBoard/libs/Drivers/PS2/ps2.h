#ifndef __PS2_H__
#define __PS2_H__

#include "main.h"
#include "servo_motor.h"
#include "bldc_motor.h"

#define FORWARD 0xCF  // R[3], R[3] FREE: 0xDF
#define BACKWARD 0x9F // R[3]
#define LEFT 0xEE     // R[4], R[4] FREE: 0xEF
#define RIGHT 0xED    // R[4]
#define RESET_STEERING 0xEB      // R[4]
#define RESET_SPEEDING 0xE7      // R[4]
// #define CAM 0xE7      // R[4]



void PS2_Data(GPIO_TypeDef *gpio, uint16_t pin, SPI_HandleTypeDef *hspi, 
                servo_motor_t *const servo_p, bldc_motor_t *const motor_p);
#endif
 