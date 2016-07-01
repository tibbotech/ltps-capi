/*!
    \example gpio.cpp
*/

#include "ltps/capi/gpio.h"


int main()
{
    Gpio gpio;

    if (gpio.getDirection("S1A") == input)
        gpio.setDirection("S1A", output);

    if (gpio.getValue("S1A") == 0)
        gpio.setValue("S1A", 1);
}
