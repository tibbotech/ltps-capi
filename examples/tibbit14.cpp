/*!
    \example tibbit14.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/dac.h"

int main()
{
    Dac dac;
    DacData data;

    for (int i = 1; i < 5; i++)
    {
        dac.setVoltage("s1", i, i * 2500, data);
        if (data.status == EXIT_FAILURE)
            printf("%s\n", data.error);
        else
            printf("Voltage %i mV for channel %i applied successfully\n", i * 2500, i);
    }

    return 0;
}
