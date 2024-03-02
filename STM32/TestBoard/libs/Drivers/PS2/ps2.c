#include "ps2.h"

uint8_t PSX_RX[8];
uint8_t PSX_TX[8] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t state_set;

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
        motor_p->speed = LOW_SPEED;
        motor_p->direction = COUNTER_CLOCKWISE;
        servo_p->set_steering(servo_p);
        motor_p->set_speed(motor_p);
        break;
    case BACKWARD:
        /* code */
        motor_p->speed = LOW_SPEED;
        motor_p->direction = CLOCKWISE;
        servo_p->set_steering(servo_p);
        motor_p->set_speed(motor_p);
        break;
    default:
        break;
    }
    static uint32_t timer = 0;
    switch (PSX_RX[4])
    {
    case LEFT:
        /* code */
        state_set = 1;
        if (HAL_GetTick() - timer > 1)
        {
            state_set = 2;
            timer = HAL_GetTick();
        }
        break;
    case RIGHT:
        /* code */
        state_set = 10;
        if (HAL_GetTick() - timer > 1)
        {
            state_set = 20;
            timer = HAL_GetTick();
        }
        break;
    case RESET_STEERING:
        /* code */
        servo_p->duty_steering = 50;
        servo_p->set_steering(servo_p);
        break;
    case RESET_SPEEDING:
        motor_p->speed = STOP_SPEED;
        motor_p->set_speed(motor_p);
        /* code */
        break;
    default:
        if (state_set == 2)
        {
            servo_p->duty_steering -= 5;
            servo_p->set_steering(servo_p);
            state_set = 0;
        }
        if (state_set == 20)
        {
            servo_p->duty_steering += 5;
            servo_p->set_steering(servo_p);
            state_set = 0;
        }
        break;
    }
}
