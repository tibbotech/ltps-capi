/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
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
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        getData(busn, hum);
}

void Humidity::getData(int busn, HumData &hum)
{
    memset(&hum, 0, sizeof hum);

    Ci2c_smbus i2c;

    int res = i2c.set_bus(busn);

    if (res != 1)
    {
        printf("Ambient humidity meter set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data[4];
    memset(&data, 0, 4);

    i2c.Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);
    memset(&data, 0, 4);

    res = i2c.Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);

    if (res != 4)
    {
        printf("Error while get data for ambient humidity meter\n");
        return;
    }

    hum.status = (data[0] >> 6);

    hum.humidity = (float) (((unsigned int) (data[0] & 0x3F) << 8) | data[1]) * 100 / (pow(2, 14) - 2);

    hum.temperature = (float) (((unsigned int) (data[2] << 6) + (data[3] >> 2)) / (pow(2, 14) - 2) * 165 - 40);
}
