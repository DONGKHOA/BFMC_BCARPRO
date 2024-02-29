#include "ps2.h"

uint8_t PSX_RX[8];
uint8_t PSX_TX[8] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void PS2_Data(GPIO_TypeDef *gpio, uint16_t pin, SPI_HandleTypeDef *hspi, 
                servo_motor_t *const servo_p, bldc_motor_t *const motor_p)
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hspi, PSX_TX, PSX_RX, 8, 100);
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);

    switch (PSX_RX[3])
    {
    case FORWARD:
        /* code */
        motor_p->timer_p->Instance->CCR1 = 80;
        break;
    case BACKWARD:
        /* code */
        motor_p->timer_p->Instance->CCR1 = 70;
        break;
    default:
        break;
    }

    switch (PSX_RX[4])
    {
    case LEFT:
        /* code */
        servo_p->duty_steering -= 5;
        motor_p->timer_p->Instance->CCR1 = 80;
        break;
    case RIGHT:
        /* code */
        servo_p->duty_steering += 5;
        motor_p->timer_p->Instance->CCR1 = 80;
        break;
    case RESET:
        /* code */
        servo_p->duty_steering = 50;
        motor_p->timer_p->Instance->CCR1 = 75;
        break;
    case CAM:
        /* code */
        break;
    default:
        break;
    }
}
 