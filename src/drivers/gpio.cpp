/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include <string>
#include <algorithm>

#include "drivers/cpin.h"
#include "lutils.h"

#include "drivers/gpio.h"

namespace GpioPrivate
{
    CPin gpio;
}

Gpio::Gpio()
{

}

Gpio::~Gpio()
{

}

int Gpio::setDirection(const char *pin, GpioDir direction)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
    {
        if (direction == input)
            return GpioPrivate::gpio.dir_set(PIN_DIR_I);
        else
            return GpioPrivate::gpio.dir_set(PIN_DIR_O);
    }
    else
        printf("GPIO PIN initialization error\n");

    return -1;
}

GpioDir Gpio::getDirection(const char *pin)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
    {
        int dir = GpioPrivate::gpio.dir_get();
        if (dir == PIN_DIR_I)
            return input;
        else
            return output;
    }
    else
        printf("GPIO PIN initialization error\n");

    return input;
}

int Gpio::setValue(const char* pin, unsigned int value)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
    {
        if (value == 0)
            return GpioPrivate::gpio.W(0);
        else
            return GpioPrivate::gpio.W(1);
    }
    else
        printf("GPIO PIN initialization error\n");

    return - 1;
}

unsigned int Gpio::getValue(const char *pin)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
    {
        int value = GpioPrivate::gpio.R();
        if (value == 0)
            return 0;
        else
            return 1;
    }
    else
        printf("GPIO PIN initialization error\n");

    return 0;
}

unsigned int Gpio::getPinNumber(const char *pin)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    return Lutils::getInstance().readInteger("CPU", spin.c_str());
}
