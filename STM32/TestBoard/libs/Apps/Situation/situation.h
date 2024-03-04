/*
 * @file situation.h
 *
 */

#ifndef APPS_SITUATION_SITUATION_H_
#define APPS_SITUATION_SITUATION_H_

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

// HEADING

#define DISTANCE_LANE 0X01
#define TRAFFIC_SIGNS 0X02
#define TRAFFIC_LIGHTS 0X03
#define BARRIER 0X04
#define INDEFINITE_LANE 0X05

// DATA

// Traffic sign
#define TRAFFIC_SIGN_STOP 0X01
#define TRAFFIC_SIGN_PARKING 0X02
#define TRAFFIC_SIGN_PRIORITY 0X03
#define TRAFFIC_SIGN_CROSSWALK 0X04
#define TRAFFIC_SIGN_HIGHWAY_ENTRANCE 0X05
#define TRAFFIC_SIGN_HIGHWAY_EXIT 0X06
#define TRAFFIC_SIGN_ROUND_ABOUT 0X07
#define TRAFFIC_SIGN_ONE_WAY 0X08
#define TRAFFIC_SIGN_NO_ENTRY 0X09

// Traffic lights
#define TRAFFIC_LIGHT_RED 0X01
#define TRAFFIC_LIGHT_YELLOW 0X02
#define TRAFFIC_LIGHT_GREEN 0X03

#define DISTANCE_BARRIER 30      // cm

// BIT Field

#define SPEEDING_BIT    (1 << 0)
#define STEERING_CONTROL_BIT    (1 << 1)
#define STEERING_NONE_CONTROL_BIT    (1 << 2)
#define CAMERA_BIT      (1 << 3)
#define BARRIER_AVOID   (1 << 4)
#define PARKING         (1 << 5)

// BARRIER_AVOID

#define LANE_CHANGE_ANGLE   45
#define STEERING_CHANGE_ANGLE 10
#define DISTANCE_BESIDE_BARRIER    15  // cm

typedef struct 
{
    uint8_t state_forward : 1;
    uint8_t state_right : 1;
    uint8_t state_back : 1;
    uint8_t state_left : 1;
} __attribute__((packed)) sr04_state_t;


void controlCarBarrier(uint8_t signs);
void controlCarParking(void);

#endif /* APPS_SITUATION_SITUATION_H_ */