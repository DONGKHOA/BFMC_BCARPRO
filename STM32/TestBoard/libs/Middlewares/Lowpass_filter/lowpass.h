#ifndef __LP_FILTER_H__
#define __LP_FILTER_H__

#define SAMPLE_RATE 100
#define pi 3.141592564
float x_Low, y_Low, z_Low, x_old, y_old, z_old;
float dtt;
float RC;

float filter(float cutofFreq);

#endif