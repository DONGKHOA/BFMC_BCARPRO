#include "ps2.h"

void PS2_Data(SPI_HandleTypeDef *hspi, servo_motor_t *const servo_p, bldc_motor_t *const motor_p)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hspi, PSX_TX, PSX_RX, 8, 100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

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
