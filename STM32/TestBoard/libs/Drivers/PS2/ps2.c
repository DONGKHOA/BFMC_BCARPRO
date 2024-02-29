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
    	motor_p->direction = COUNTER_CLOCKWISE;
        servo_p->set_steering(servo_p);
        motor_p->set_speed(motor_p);
        break;
    case BACKWARD:
        /* code */
    	motor_p->direction = CLOCKWISE;
        servo_p->set_steering(servo_p);
        motor_p->set_speed(motor_p);
        break;
    default:
        break;
    }

    switch (PSX_RX[4])
    {
    case LEFT:
        /* code */
        servo_p->duty_steering -= 5;
        servo_p->set_steering(servo_p);
        break;
    case RIGHT:
        /* code */
        servo_p->duty_steering += 5;
        servo_p->set_steering(servo_p);
        break;
    case RESET:
        /* code */
        servo_p->duty_steering = 50;
        servo_p->set_steering(servo_p);
        break;
    case CAM:
        /* code */
        break;
    default:
        break;
    }
}