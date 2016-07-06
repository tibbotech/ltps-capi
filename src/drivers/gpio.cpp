/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include <string>
#include <algorithm>

#include "cpin.h"
#include "lutils.h"

#include "gpio.h"

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

int Gpio::setDirection(const char *pin, int direction)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
        return GpioPrivate::gpio.dir_set(direction);
    else
        printf("GPIO PIN initialization error\n");

    return -1;
}

int Gpio::getDirection(const char *pin)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
        return GpioPrivate::gpio.dir_get();
    else
        printf("GPIO PIN initialization error\n");

    return PIN_DIR_I;
}

int Gpio::setValue(const char* pin, unsigned int value)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
        return GpioPrivate::gpio.W(value);
    else
        printf("GPIO PIN initialization error\n");

    return - 1;
}

unsigned int Gpio::getValue(const char *pin)
{
    std::string spin(pin);
    std::transform(spin.begin(), spin.end(), spin.begin(), ::toupper);

    if (!GpioPrivate::gpio.init(Lutils::getInstance().readInteger("CPU", spin.c_str())))
        return GpioPrivate::gpio.R();
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
