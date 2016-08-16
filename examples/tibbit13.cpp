/*!
    \example tibbit13.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/adc.h"

int main()
{
    Adc adc;
    AdcData data;

    for (int i = 1; i < 5; i++)
    {
        adc.getVoltage("s1", i, false, data); // Set the third argument in the true to get previous voltage conversion (1 reading cycle)

        if (data.status == EXIT_SUCCESS)
            printf("Voltage for channel %i = %i mV\n", i, data.voltage);
        else
            printf("%s", data.error);
    }

    return 0;
}
