/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include <map>

#include "cpin.h"
#include "lutils.h"

#include "gpio.h"

Gpio::Gpio()
{

}

Gpio::~Gpio()
{

}

int Gpio::setDirection(const char *pin, int direction)
{
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin);

    if (cpin)
        cpin->dir_set(direction);
    else
        printf("GPIO set direction error for socket %s\n", pin);

    return -1;
}

int Gpio::getDirection(const char *pin)
{
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin);

    if (cpin)
        return cpin->dir_get();
    else
        printf("GPIO set direction error for socket %s\n", pin);

    return PIN_DIR_I;
}

int Gpio::setValue(const char* pin, unsigned int value)
{
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin);

    if (cpin)
        return cpin->W(value);
    else
        printf("GPIO set direction error for socket %s\n", pin);

    return -1;
}

unsigned int Gpio::getValue(const char *pin)
{
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin);

    if (cpin)
        return cpin->R();
    else
        printf("GPIO set direction error for socket %s\n", pin);

    return 0;
}

unsigned int Gpio::getPinNumber(const char *pin)
{
    return Lutils::getInstance().readInteger("CPU", pin);
}
