/*!
    \example tibbit36.cpp
*/

#include <stdio.h> // printf

#include "ltps/accelerometer.h"

int main()
{
    Accelerometer accel;
    AccelData data;

    accel.getData("s1", data);

    printf("Acceleration X-axis = %i mG\n", data.lx);
    printf("Acceleration Y-axis = %i mG\n", data.ly);
    printf("Acceleration Z-axis = %i mG\n", data.lz);
}
