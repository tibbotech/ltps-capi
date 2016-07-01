/*!
    \example gpio.cpp
*/

#include "ltps/capi/gpio.h"
#include "ltps/capi/cpin.h" // Defines

int main()
{
    Gpio gpio;

    if (gpio.getDirection("S1A") == PIN_DIR_I)
        gpio.setDirection("S1A", PIN_DIR_O);

    if (gpio.getValue("S1A") == 0)
        gpio.setValue("S1A", 1);
}
