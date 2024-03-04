#include "spi.h"

extern SR04Driver_t *hc_sr04_p;

void control_ras(SPI_HandleTypeDef *hspi, GPIO_TypeDef *gpio, uint8_t pin)
{
    hspi->Instance->CR1 = (hspi->Instance->CR1) & 0x003F;
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    osDelay(10);
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
    hspi->Instance->CR1 = (hspi->Instance->CR1) & 0x00FF;
}

void wait_control_ras(SPI_HandleTypeDef *hspi, GPIO_TypeDef *gpio, uint8_t pin, SR04Driver_t *hc_sr04_p)
{
    if ((hc_sr04_p->get_distance(hc_sr04_p)) >= 30)
        control_ras(hspi, gpio, pin);
}

void SPIcontrol_Init(SPIcontrol_t *spicon_p, void (*get_data_func)(SPI_HandleTypeDef *hspi, GPIO_TypeDef *gpio, uint8_t pin, SR04Driver_t *hc_sr04_p))
{
    spicon_p->wait_control = get_data_func;
}

SPIcontrol_t *SPIcontrol_Create()
{
    SPIcontrol_t *spicon_p = malloc(sizeof(SPIcontrol_t));
    if (spicon_p != NULL)
    {
        SPIcontrol_Init(spicon_p, wait_control_ras);
    }
    return spicon_p;
}

void SPIcontrol_Destroy(SPIcontrol_t *const spicon_p)
{
    free(spicon_p);
}
