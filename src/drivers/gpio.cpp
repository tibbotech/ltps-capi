/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include <map>

#include "cpin.h"
#include "lutils.h"

#include "gpio.h"

namespace GpioPrivate
{
    std::map<const char*, CPin*, CompareCStrings> pins;

    void checkGpioPin(const char* pin)
    {
        if (GpioPrivate::pins.find(pin) == GpioPrivate::pins.end())
        {
            CPin *cpin = new CPin();
            int res = cpin->init(Lutils::getInstance().readInteger("CPU", pin));
            if (!res)
            {
                char *data = new char(strlen(pin) + 1);
                strcpy(data, pin);

                GpioPrivate::pins[data] = cpin;
            }
            else
                printf("GPIO PIN initialization error: %s\n", strerror(abs(res)));
        }
    }
}

Gpio::Gpio()
{

}

Gpio::~Gpio()
{
    for (std::map<const char*, CPin*, CompareCStrings>::iterator it = GpioPrivate::pins.begin(); it != GpioPrivate::pins.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }
}

int Gpio::setDirection(const char *pin, int direction)
{
    GpioPrivate::checkGpioPin(pin);

    if (GpioPrivate::pins.find(pin) != GpioPrivate::pins.end())
        return GpioPrivate::pins.at(pin)->dir_set(direction);
    else
        printf("GPIO set direction error for socket %s\n", pin);

    return -1;
}

int Gpio::getDirection(const char *pin)
{
    GpioPrivate::checkGpioPin(pin);

    if (GpioPrivate::pins.find(pin) != GpioPrivate::pins.end())
        return GpioPrivate::pins.at(pin)->dir_get();
    else
        printf("GPIO get direction error for socket %s\n", pin);

    return PIN_DIR_I;
}

int Gpio::setValue(const char* pin, unsigned int value)
{
    GpioPrivate::checkGpioPin(pin);

    if (GpioPrivate::pins.find(pin) != GpioPrivate::pins.end())
        return GpioPrivate::pins.at(pin)->W(value);
    else
        printf("GPIO set value error for socket %s\n", pin);

    return - 1;
}

unsigned int Gpio::getValue(const char *pin)
{
    GpioPrivate::checkGpioPin(pin);

    if (GpioPrivate::pins.find(pin) != GpioPrivate::pins.end())
        return GpioPrivate::pins.at(pin)->R();
    else
        printf("GPIO get value error for socket %s\n", pin);

    return 0;
}

unsigned int Gpio::getPinNumber(const char *pin)
{
    return Lutils::getInstance().readInteger("CPU", pin);
}
