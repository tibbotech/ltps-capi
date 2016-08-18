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
    const char* lsb[] =
    {
        "S1A",  "S1B",  "S1C",  "S1D",
        "S3A",  "S3B",  "S3C",  "S3D",
        "S5A",  "S5B",  "S5C",  "S5D",
        "S7A",  "S7B",  "S7C",  "S7D",
        "S9A",  "S9B",  "S9C",  "S9D",
        "S11A", "S11B", "S11C", "S11D",
        "S13A", "S13B", "S13C", "S13D",
        "S15A", "S15B", "S15C", "S15D"
    };

    const char* msb[] =
    {
        "S17A", "S17B", "S17C", "S17D",
        "S19A", "S19B", "S19C", "S19D",
        "S21A", "S21B", "S21C", "S21D",
        "S23A", "S23B", "S23C", "S23D",
        "S25A", "S25B", "S25C"
    };

    inline void setData(int lsbPins, int msbPins, int lsbData, int msbData, bool direction)
    {
        for (unsigned int n = 0; n < sizeof(GpioPrivate::lsb) / sizeof(const char*); n++)
        {
            if ((lsbPins >> n) & 1)
            {
                char *error;
                CPin* cpin = Lutils::getInstance().getGpioPointer(GpioPrivate::lsb[n], &error);

                if (cpin)
                {
                    if (direction) cpin->dir_set((lsbData >> n) & 1);
                    else cpin->W((lsbData >> n) & 1);
                }
            }
        }

        for (unsigned int n = 0; n < sizeof(GpioPrivate::msb) / sizeof(const char*); n++)
        {
            if ((msbPins >> n) & 1)
            {
                char *error;
                CPin* cpin = Lutils::getInstance().getGpioPointer(GpioPrivate::msb[n], &error);

                if (cpin)
                {
                    if (direction) cpin->dir_set((msbData >> n) & 1);
                    else cpin->W((msbData >> n) & 1);
                }
            }
        }
    }
}

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

void Gpio::setDirection(int lsbPins, int msbPins, int lsbDirs, int msbDirs)
{
    GpioPrivate::setData(lsbPins, msbPins, lsbDirs, msbDirs, true);
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

void Gpio::setValue(int lsbPins, int msbPins, int lsbValues, int msbValues)
{
    GpioPrivate::setData(lsbPins, msbPins, lsbValues, msbValues, false);
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
