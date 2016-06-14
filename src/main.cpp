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
        if (atoi(argv[1]) == 13) //< ADC (Tibbit #13)
        {
            Adc adc;

            printf("Channel 1 = %i mV\n", adc.getVoltage(argv[2], 1));
            printf("Channel 2 = %i mV\n", adc.getVoltage(argv[2], 2));
            printf("Channel 3 = %i mV\n", adc.getVoltage(argv[2], 3));
            printf("Channel 4 = %i mV\n", adc.getVoltage(argv[2] ,4));
        }

        if (atoi(argv[1]) == 14) //< DAC (Tibbit #14)
        {
            Dac dac;

            printf("Trying set channel 1 to 2500 mV\n");
            dac.setVoltage(argv[2], 1, 2500);

            printf("Trying set channel 2 to 5000 mV\n");
            dac.setVoltage(argv[2], 2, 5000);

            printf("Trying set channel 3 to -7500 mV\n");
            dac.setVoltage(argv[2], 3, -7500);

            printf("Trying set channel 4 to 10000 mV\n");
            dac.setVoltage(argv[2], 4, 10000);
        }

        if (atoi(argv[1]) == 28) //< Ambient light sensor (Tibbit #28)
        {
            Light lt;

            printf("Illumination = %i lx\n", lt.getIllumination(argv[2]));
        }

        if (atoi(argv[1]) == 30) //< Ambient humidity meter (Tibbit #30)
        {
            Humidity hum;

            Hih6130 data = hum.getData(argv[2]);

            printf("RH = %f percents\n", data.humidity);
            printf("Temperature = %f degrees Celsius\n", data.temperature);
            printf("Status = %i\n", data.status);
        }

        if (atoi(argv[1]) == 36) //< 3-axis accelerometer (Tibbit #36)
        {
            Accelerometer accel;

            Adxl312 data = accel.getData(argv[2]);

            printf("X = %i mG\n", data.lx);
            printf("Y = %i mG\n", data.ly);
            printf("Z = %i mG\n", data.lz);
        }
    }
}
