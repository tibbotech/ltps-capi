/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "ltps.h"

#include "string"
#include "global.h"

#if !defined(_GNU_SOURCE)
    #define _GNU_SOURCE
#endif

#ifdef UNUSED
#elif __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#include "drivers/gpio.h"

int main(int UNUSED(argc), char *UNUSED(argv[]))
{
    Gpio gpio;

    if (gpio.getDirection("S1A") == input)
        gpio.setDirection("S1A", output);

    if (gpio.getValue("S1A") == 0)
        gpio.setValue("S1A", 1);
}
