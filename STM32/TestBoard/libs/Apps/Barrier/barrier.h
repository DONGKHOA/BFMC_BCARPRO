/*
 * @file barrier.h
 *
 */

#ifndef APPS_BARRIER_BARRIER_H_
#define APPS_BARRIER_BARRIER_H_

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdlib.h>
#include "main.h"

/*********************
 *      DEFINES
 *********************/

#define AVOID_BARRIER_ANGLE 30;
#define COUNTER_CLOCKWISE;

/**********************
 *      TYPEDEFS
 **********************/

struct control_barrier;
typedef struct control_barrier control_barrier_t;

struct control_barrier
{
    float (* set_control)(control_barrier_t *const control_p);
};

/**********************
 *     OPERATION
 **********************/

control_barrier_t *BARRIER_CONTROL_Create();
void BARRIER_CONTROL_Destroy(control_barrier_t * const control_p);

#endif /* APPS_BARRIER_BARRIER_H_ */
