/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "Ci2c_smbus.h"
#include "tibbits/i2c/light.h"

#include "global.h"
#include "lutils.h"

namespace LightPrivate
{
    Ci2c_smbus i2c;
}

Light::Light()
{

}

Light::~Light()
{

}

int Light::getIllumination(const char *socket)
{
    int busn = Lutils::getInstance().getI2CBusNum(socket);

    if (busn == -1)
    {
        printf("I2C bus for socket %s not found\n", socket);
        return 0;
    }
    else
        return getIllumination(busn);
}

int Light::getIllumination(int bus)
{
    int res = LightPrivate::i2c.set_bus(bus);

    if (res != 1)
    {
        printf("Ambient light sensor set I2C bus errno: %i\n", res);
        return 0;
    }

    uint8_t data[2];
    memset(&data, 0, 2);

    res = LightPrivate::i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWEROFF, data, 0);

    res += LightPrivate::i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWERON, data, 0);

    res += LightPrivate::i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::HIGHRES, data, 0);

    usleep(180000);

    res += LightPrivate::i2c.Rbb(BH1721FVC::I2C_ADDRESS, 0x00, data, 2);

    if (res != 2)
    {
        printf("Error while get illumination for ambient light sensor\n");
        return 0;
    }

    return (data[0] * 256 + data[1]) / 1.2;
}
