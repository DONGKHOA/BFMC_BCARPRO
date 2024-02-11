/*
 * @file distance_handle.h
 *
 */
#ifndef _DISTANCE_HANDLE_H_
#define _DISTANCE_HANDLE_H_

#include "main.h"
#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

/**********************
 *      CONSTANTS
 **********************/

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
    float (*set_interpolate)(distance_handle_t *const distance_handle_p);
};

/**********************
 *     OPERATION
 **********************/

distance_handle_t *DISTANCE_HANDLE_Create();
void DISTANCE_HANDLE_Destroy(distance_handle_t * const distance_handle_p);

#endif
