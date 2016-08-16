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
    char *error;
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin, &error);

    if (cpin)
        cpin->dir_set(direction);
    else
        printf("GPIO set direction error for socket %s - %s\n", pin, error);

    return -1;
}

int Gpio::getDirection(const char *pin)
{
    char *error;
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin, &error);

    if (cpin)
        return cpin->dir_get();
    else
        printf("GPIO set direction error for socket %s - %s\n", pin, error);

    return PIN_DIR_I;
}

int Gpio::setValue(const char* pin, unsigned int value)
{
    char *error;
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin, &error);

    if (cpin)
        return cpin->W(value);
    else
        printf("GPIO set direction error for socket %s - %s\n", pin, error);

    return -1;
}

unsigned int Gpio::getValue(const char *pin)
{
    char *error;
    CPin* cpin = Lutils::getInstance().getGpioPointer(pin, &error);

    if (cpin)
        return cpin->R();
    else
        printf("GPIO set direction error for socket %s - %s\n", pin, error);

    return 0;
}

unsigned int Gpio::getPinNumber(const char *pin)
{
    return Lutils::getInstance().readInteger("CPU", pin);
}
