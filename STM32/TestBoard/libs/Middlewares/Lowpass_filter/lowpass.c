#include "lowpass.h"

float filter(float cutofFreq)
{
    float RC = 1.0/(cutofFreq * 2 * pi);
    float dtt = 1.0/SAMPLE_RATE;
    float alpha = dtt/(RC+dtt);

    return alpha;
}