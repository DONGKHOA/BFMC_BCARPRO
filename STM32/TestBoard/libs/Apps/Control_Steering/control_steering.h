/*
 * @file fuzzy_control.h
 *
 */

#ifndef APPS_CONTROL_STEERING_H_
#define APPS_CONTROL_STEERING_H_

/*********************
 *      INCLUDES
 *********************/
#include "stdint.h"
#include "stdlib.h"
#include "main.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef __cplusplus
    }
#endif

/*********************
 *      DEFINES
 *********************/
#define VELOCITY_GAIN 26.8f
#define DISTANCE_GAIN 10.0f
#define DUTY_GAIN 25.0f
#define MIDDLE_DUTY 50.0f

/**********************
 *      TYPEDEFS
 **********************/

struct control_steering;
typedef struct control_steering control_steering_t;
struct control_steering
{
    float input[2];
    float output;
    uint32_t (* set_control)(control_steering_t *const control_p, float distance, uint32_t duty_speed);
};

/**********************
 *     OPERATION
 **********************/
void Fuzzy_init(control_steering_t * const control_p,
				uint32_t (*set_control_func)(control_steering_t *const control_p, float distance, uint32_t duty_speed));
void Fuzzy_run(float *inputs, float *outputs);
control_steering_t *CONTROL_STEERING_Create();
void CONTROL_STEERING_Destroy(control_steering_t * const control_p);

#endif /* APPS_CONTROL_STEERING_H_ */
