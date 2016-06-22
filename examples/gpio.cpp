/*!
    \example gpio.cpp
*/

#include <stdio.h> // printf

#include "ltps/cpin.h"
#include "ltps/lutils.h"

int main()
{
    CPin gpio;
    if (!gpio.init(Lutils::readInteger(PINS_INI_FILE, "CPU", "S1A"))) //< Successful initialization
    {
        if (gpio.dir_get() == PIN_DIR_I)
            gpio.dir_set(PIN_DIR_O);

        printf("Set PIN value to 1\n");
        gpio.W(1);

        printf("PIN value = %i\n", gpio.R());
    }
}
