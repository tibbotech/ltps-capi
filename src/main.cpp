/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "ltps.h"

#if !defined(_GNU_SOURCE)
    #define _GNU_SOURCE
#endif

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage with I2C Tibbit's: %s tibbit_number socket\n", argv[0]);
        printf("For example: %s 36 s1\n", argv[0]);
    }

    if (argc == 3) //< I2C Tibbit's
    {
        if (atoi(argv[1]) == 36) //< #36: 3-axis accelerometer
        {
            Accelerometer accel(argv[2]);

            Adxl312 data = accel.getData();

            printf("X = %i mG\n", data.lx);
            printf("Y = %i mG\n", data.ly);
            printf("Z = %i mG\n", data.lz);
        }
    }
}
