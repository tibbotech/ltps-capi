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
            printf("ADC (Tibbit #13) selected\n");

            Adc adc;

            printf("Channel 1 = %i mV\n", adc.getVoltage(argv[2], 1));
            printf("Channel 2 = %i mV\n", adc.getVoltage(argv[2], 2));
            printf("Channel 3 = %i mV\n", adc.getVoltage(argv[2], 3));
            printf("Channel 4 = %i mV\n", adc.getVoltage(argv[2] ,4));
        }

        if (atoi(argv[1]) == 14) //< DAC (Tibbit #14)
        {
            printf("DAC (Tibbit #14) selected\n");

            Dac dac;

            printf("Trying to set channel 1 to 2500 mV\n");
            dac.setVoltage(argv[2], 1, 2500);

            printf("Trying to set channel 2 to 5000 mV\n");
            dac.setVoltage(argv[2], 2, 5000);

            printf("Trying to set channel 3 to -7500 mV\n");
            dac.setVoltage(argv[2], 3, -7500);

            printf("Trying to set channel 4 to 10000 mV\n");
            dac.setVoltage(argv[2], 4, 10000);
        }

        if ((atoi(argv[1]) == 16) ||
                (atoi(argv[1]) == 17)) //< PIC (PWM) (Tibbit #16, Tibbit #17) (Tibbit #31 also supported PWM)
        {
            printf("PIC-based Tibbit (Tibbit #%i) selected\n", atoi(argv[1]));

            Pic pic;

            printf("Trying to initialize PIC\n");
            pic.initPic(argv[2], _32MHz);

            for (int i = 1; i < 4; i++)
            {
                printf("Trying to configure PIC as PWM at channel %i\n", i);
                pic.configurePwm(argv[2], i);
                sleep(1);

                printf("Trying to start PWM at channel %i (pusle = %i, period = %i, prescaler = %i)\n", i, 100, i * 300, 1);
                pic.startPwm(argv[2], i, 100, i * 300, 1);
                sleep(3);

                printf("Trying to stop PWM at channel %i\n", i);
                pic.stopPwm(argv[2], i);
            }
        }

        if (atoi(argv[1]) == 28) //< Ambient light sensor (Tibbit #28)
        {
            printf("Ambient light sensor (Tibbit #28) selected\n");

            Light lt;

            printf("Illumination = %i lx\n", lt.getIllumination(argv[2]));
        }

        if (atoi(argv[1]) == 30) //< Ambient humidity meter (Tibbit #30)
        {
            printf("Ambient humidity meter (Tibbit #30) selected\n");

            Humidity hum;

            Hih6130 data = hum.getData(argv[2]);

            printf("RH = %f percents\n", data.humidity);
            printf("Temperature = %f degrees Celsius\n", data.temperature);
            printf("Status = %i\n", data.status);
        }

        if (atoi(argv[1]) == 31) //< PIC (ADC) (Tibbit #31)
        {
            printf("PIC-based Tibbit (Tibbit #%i) selected\n", atoi(argv[1]));

            Pic pic;

            printf("Trying to initialize PIC\n");
            pic.initPic(argv[2], _32MHz);

            for (int i = 1; i < 5; i++)
            {
                printf("Trying to configure PIC as ADC at channel %i\n", i);
                pic.configureAdc(argv[2], i);
                sleep(1);

                printf("Channel %i = %i mV\n", i, pic.getAdcVoltage(argv[2], i));
            }
        }

        if (atoi(argv[1]) == 36) //< 3-axis accelerometer (Tibbit #36)
        {
            printf("3-axis accelerometer (Tibbit #36) selected\n");

            Accelerometer accel;

            Adxl312 data = accel.getData(argv[2]);

            printf("X = %i mG\n", data.lx);
            printf("Y = %i mG\n", data.ly);
            printf("Z = %i mG\n", data.lz);
        }

        if (atoi(argv[1]) == 41) //< 8-bit port extender (Tibbit #41)
        {
            printf("8-bit port extender (Tibbit #41) selected\n");

            Portextender pext;

            Mcp23008 data;
            data.direction = false;
            data.pullup = false; //< It does not matter for output mode (see Mcp23008 struct definition)

            for (int i = 1; i < 9; i++)
            {
                printf("Setting pin %i to output with value = %i\n", i, i % 2);
                data.value = i % 2;
                pext.setData(argv[2], i, data);
            }
        }
    }
}
