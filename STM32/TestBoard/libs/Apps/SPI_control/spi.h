#ifndef SPI_APP_H_
#define SPI_APP_H_

#include "main.h"
#include "hc_sr04.h"
#include <stdlib.h>

typedef struct SPIcontrol SPIcontrol_t;
struct SPIcontrol
{
    void (*wait_control)(SPI_HandleTypeDef *hspi, GPIO_TypeDef *gpio, uint8_t pin, SR04Driver_t *hc_sr04_p);
};

SPIcontrol_t *SPIcontrol_Create();
void SPIcontrol_Destroy(SPIcontrol_t *const spicon_p);

#endif
