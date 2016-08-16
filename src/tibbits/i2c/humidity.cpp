/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/humidity.h"

#include "global.h"
#include "lutils.h"

#include <math.h>

Humidity::Humidity()
{

}

Humidity::~Humidity()
{

}

void Humidity::getData(const char* socket, HumData &hum)
{
    memset(&hum, 0, sizeof hum);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        uint8_t data[4];
        memset(&data, 0, 4);

        i2c->Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);
        memset(&data, 0, 4);

        int res = i2c->Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);

        if (res != 4)
        {
            hum.status = EXIT_FAILURE;
            hum.error = "Checksum error while get data for ambient humidity meter";
            return;
        }

        // hum.status = (data[0] >> 6);
        hum.humidity = (float) (((unsigned int) (data[0] & 0x3F) << 8) | data[1]) * 100 / (pow(2, 14) - 2);
        hum.temperature = (float) (((unsigned int) (data[2] << 6) + (data[3] >> 2)) / (pow(2, 14) - 2) * 165 - 40);

        hum.status = EXIT_SUCCESS;
    }
    else
    {
        hum.status = EXIT_FAILURE;
        hum.error = error;
    }
}
