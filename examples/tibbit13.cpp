/*!
    \example tibbit13.cpp
*/

#include <stdio.h> // printf

#include "ltps/capi/adc.h"

int main()
{
    Adc adc;

    printf("Voltage for channel 1 = %i mV\n", adc.getVoltage("s1", 1));
    printf("Voltage for channel 2 = %i mV\n", adc.getVoltage("s1", 2));
    printf("Voltage for channel 3 = %i mV\n", adc.getVoltage("s1", 3));
    printf("Voltage for channel 4 = %i mV\n", adc.getVoltage("s1" ,4));

    return 0;
}
