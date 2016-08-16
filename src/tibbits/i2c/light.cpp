/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/light.h"

#include "global.h"
#include "lutils.h"

Light::Light()
{

}

Light::~Light()
{

}

void Light::getIllumination(const char *socket, LightData &ill)
{
    memset(&ill, 0, sizeof ill);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        uint8_t data[2];
        memset(&data, 0, 2);

        int res = i2c->Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWEROFF, data, 0);

        res += i2c->Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWERON, data, 0);

        res += i2c->Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::HIGHRES, data, 0);

        usleep(180000);

        res += i2c->Rbb(BH1721FVC::I2C_ADDRESS, 0x00, data, 2);

        if (res != 2)
        {
            ill.status = EXIT_FAILURE;
            ill.error = "Checksum error while get illumination for ambient light sensor";
            return;
        }

        ill.ill = (data[0] * 256 + data[1]) / 1.2;
        ill.status = EXIT_SUCCESS;
    }
    else
    {
        ill.status = EXIT_FAILURE;
        ill.error = error;
    }

}
