/*
 * @file fuzzy_control.h
 *
 */

#ifndef MIDDLEWARES_PID_CONTROL_H_
#define MIDDLEWARES_PID_CONTROL_H_

/*********************
 *      INCLUDES
 *********************/
#include "stdint.h"
#include "stdlib.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define KP_VALUE 10.0f;
#define KI_VALUE 10.0f;
#define KD_VALUE 10.0f;
#define TS_VALUE 0.05f;
#define INF_VALUE 75;
#define SUP_VALUE 25;

/**********************
 *      TYPEDEFS
 **********************/

struct control_steering;
typedef struct control_steering control_steering_t;

struct control_steering
{
    float Kp;
    float Ki;
    float Kd;
    float error;
    float pre_error;
    float pre2_error;
    float Ts;
    float Kp_part;
    float Ki_part;
    float Kd_part;
    float Out;
    float pre_Out;
    float angle0;
    float angle1;
    float reference;
    uint8_t Inf_Saturation;
    uint8_t Sup_Saturation;
    uint32_t (* set_control)(control_steering_t *const control_p);
};

/**********************
 *     OPERATION
 **********************/

control_steering_t *PID_CONTROL_Create();
void PID_CONTROL_Destroy(control_steering_t * const control_p);

#endif /* MIDDLEWARES_FUZZY_CONTROL_FUZZY_CONTROL_H_ */
