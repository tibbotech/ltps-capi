/*!
    \example tibbit52.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/adc-16bits.h"

int main()
{
    Adc16 adc;
    Adc16Data data;

    for (int i = 1; i < 5; i++)
    {
        adc.getVoltage("s1", i, data);

        if (data.status == EXIT_SUCCESS)
            printf("Voltage for channel %i = %f V\n", i, data.voltage);
        else
            printf("%s\n", data.error);
    }

    return 0;
}
