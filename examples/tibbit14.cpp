/*!
    \example tibbit14.cpp
*/

#include <stdio.h> // printf

#include "ltps/capi/dac.h"

int main()
{
    Dac dac;

    printf("Set 2500 mV for channel 1\n");
    dac.setVoltage("s1", 1, 2500);

    printf("Set 5000 mV for channel 2\n");
    dac.setVoltage("s1", 2, 5000);

    printf("Set -7500 mV for channel 3\n");
    dac.setVoltage("s1", 3, -7500);

    printf("Set 10000 mV for channel 4\n");
    dac.setVoltage("s1", 4, 10000);

    return 0;
}
