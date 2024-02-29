#ifndef __PS2_H__
#define __PS2_H__

#include "main.h"
#include "servo_motor.h"
#include "bldc_motor.h"

#define FORWARD 0xCF  // R[3], R[3] FREE: 0xDF
#define BACKWARD 0x9F // R[3]
#define LEFT 0x6F     // R[4], R[4] FREE: 0xEF
#define RIGHT 0xCF    // R[4]
#define RESET 0xEB      // R[4]
#define CAM 0xE7      // R[4]

uint8_t PSX_RX[8];
uint8_t PSX_TX[8] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void PS2_Data(GPIO_TypeDef *gpio, uint16_t pin, SPI_HandleTypeDef *hspi, servo_motor_t *const servo_p, bldc_motor_t *const motor_p);
#endif
 