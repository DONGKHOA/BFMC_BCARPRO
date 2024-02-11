#include "test.h"
#include "stdio.h"

int test1()
{
    float a;
    distance_handle_t *distance_handle_0;
    distance_handle_0 = DISTANCE_HANDLE_Create();
    distance_handle_0->distance = 8;
    if (distance_handle_0 != NULL)
    {
        a = distance_handle_0->set_interpolate(distance_handle_0);
        printf("%f", a);
        DISTANCE_HANDLE_Destroy(distance_handle_0);
    }
    else
    {
        printf("Error");
    }
    return 0;
}