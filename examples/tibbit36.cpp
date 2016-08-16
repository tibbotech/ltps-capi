/*!
    \example tibbit36.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/accelerometer.h"

int main()
{
    Accelerometer accel;
    AccelData data;

    accel.getData("s1", data);

    if (data.status == EXIT_SUCCESS)
    {
        printf("Acceleration X-axis = %i mG\n", data.lx);
        printf("Acceleration Y-axis = %i mG\n", data.ly);
        printf("Acceleration Z-axis = %i mG\n", data.lz);
    }
    else
        printf("%s\n", data.error);

    return 0;
}
