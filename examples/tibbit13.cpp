/*!
    \example tibbit13.cpp
*/

#include <stdio.h> // printf

#include "ltps/capi/adc.h"

int main()
{
    Adc adc;

    printf("Voltage for channel 1 = %i mV\n", adc.getVoltage("s1", 1)); // Get actual voltage value (2 reading cycles)
    printf("Voltage for channel 2 = %i mV\n", adc.getVoltage("s1", 2)); // Get actual voltage value (2 reading cycles)
    printf("Voltage for channel 3 = %i mV\n", adc.getVoltage("s1", 3, false)); // Get actual voltage value (2 reading cycles) (false is by default)
    printf("Voltage for channel 4 = %i mV\n", adc.getVoltage("s1", 4, true)); // Get previous voltage conversion (1 reading cycle)

    return 0;
}
