#include "distance_handle.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


/**
 * The function `DISTANCE_HANDLE_Set_Interpolate` calculates the steering value based on the distance
 * value.
 * 
 * @param distance_handle_p A pointer to a structure of type distance_handle_t, which contains the
 * following members:
 * 
 * @return either 0 or the calculated steering value.
 */
void DISTANCE_HANDLE_Set_Interpolate(distance_handle_t *const distance_handle_p)
{
    if (distance_handle_p->distance > 0)
    {
        if (distance_handle_p->distance <= distanceValueP[0])
            return 0;
        for (uint8_t i = 0; i < distance_handle_p->SIZE; i++)
        {
            if ((distance_handle_p->distance >= distanceValueP[i]) && distance_handle_p->distance <= distanceValueP[i + 1])
            {
                distance_handle_p->steering = ((distance_handle_p->distance - distanceValueP[i])*steeringValueP[i + 1] + (distanceValueP[i + 1] - distance_handle_p->distance)*steeringValueP[i])/(distanceValueP[i + 1] - distanceValueP[i]);
                return distance_handle_p->steering;
            }
        }
    }
    if (distance_handle_p->distance < 0)
    {
        if (distance_handle_p->distance >= distanceValueN[0])
            return 0;
        for (uint8_t i = 0; i < distance_handle_p->SIZE; i++)
        {
            if ((distance_handle_p->distance <= distanceValueN[i]) && distance_handle_p->distance >= distanceValueN[i + 1])
            {
                distance_handle_p->steering = ((distance_handle_p->distance - distanceValueN[i])*steeringValueN[i + 1] + (distanceValueN[i + 1] - distance_handle_p->distance)*steeringValueN[i])/(distanceValueN[i + 1] - distanceValueN[i]);
                return distance_handle_p->steering;
            }
        }
    }

    // Boundary conditions for positive and negative reference values
    if (distance_handle_p->distance >= distanceValueP[distance_handle_p->SIZE - 1])
        return steeringValueP[distance_handle_p->SIZE - 1];
    if (distance_handle_p->distance <= distanceValueN[distance_handle_p->SIZE - 1])
        return steeringValueN[distance_handle_p->SIZE - 1];
}

/**
 * The function initializes a distance handle structure by assigning a function pointer to a member
 * variable.
 * 
 * @param distance_handle_p A pointer to a distance_handle_t structure. This structure likely contains
 * data and functions related to handling distance calculations.
 * @param set_interpolate_func A function pointer that points to a function that takes a
 * distance_handle_t pointer as a parameter and returns void. This function is responsible for setting
 * the interpolate function of the distance_handle_t object.
 */
void DISTANCE_HANDLE_Init(distance_handle_t *const distance_handle_p,
                            void (* set_interpolate_func)(distance_handle_t *const distance_handle_p))
{
    distance_handle_p->set_interpolate = set_interpolate_func;
    distance_handle_p->SIZE = 2;
}

/**
 * The function creates and initializes a distance handle object.
 * 
 * @return a pointer to a distance_handle_t structure.
 */
distance_handle_t *DISTANCE_HANDLE_Create()
{
    distance_handle_t *distance_handle_p = malloc(sizeof(distance_handle_t));
    if (distance_handle_p != NULL)
    {
        DISTANCE_HANDLE_Init(distance_handle_p, DISTANCE_HANDLE_Set_Interpolate);
    }
    return distance_handle_p;
}

/**
 * The function `DISTANCE_HANDLE_Destroy` is used to free the memory allocated for a distance handle.
 * 
 * @param distance_handle_p A pointer to a distance_handle_t structure that needs to be destroyed.
 */
void DISTANCE_HANDLE_Destroy(distance_handle_t * const distance_handle_p)
{
    free(distance_handle_p);
}