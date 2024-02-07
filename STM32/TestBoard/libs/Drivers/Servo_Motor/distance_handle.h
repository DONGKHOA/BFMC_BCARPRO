/*
 * @file distance_handle.h
 *
 */
#ifndef _DISTANCE_HANDLE_H_
#define _DISTANCE_HANDLE_H_

#include "main.h"
#include "stdint.h"
#include "stdlib.h"

/**********************
 *      CONSTANTS
 **********************/

/* Distance table */
const float distanceValueP[2] = {7.0, 10.0};
const float distanceValueN[2] = {-7.0, -10.0};
/* Angle table */
const float steeringValueP[2] = {15.0, 20.0};
const float steeringValueN[2] = {-15.0, -20.0};

/**********************
 *      TYPEDEFS
 **********************/

struct distance_handle;
typedef struct distance_handle distance_handle_t;

struct distance_handle
{
    uint8_t SIZE;
    float distance;
    float steering;
    /* Function for Interpolate */
    void (*set_interpolate)(distance_handle_t *const distance_handle_p);
};

/**********************
 *     OPERATION
 **********************/

distance_handle_t *DISTANCE_HANDLE_Create();
void DISTANCE_HANDLE_Destroy(distance_handle_t * const distance_handle_p);

#endif