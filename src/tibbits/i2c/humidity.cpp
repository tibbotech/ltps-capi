/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/humidity.h"

#include "global.h"
#include "utilities.h"

#include <math.h>

Humidity::Humidity()
{

}

Humidity::~Humidity()
{

}

Hih6130 Humidity::getData(const char* socket)
{
    Hih6130 hum;
    memset(&hum, 0, sizeof hum);

    std::string sock(socket);
    std::string hwSocket = Lutilites::readString(PINS_FILE, "I2C", "S" + sock.substr(1, sock.length() - 1));

    int res;
    Ci2c_smbus i2c;

    if (hwSocket.empty()) //< Software I2C
        res = i2c.set_bus(Lutilites::getI2CName(sock).c_str());
    else //< Hardware I2C
        res = i2c.set_bus(atoi(hwSocket.c_str()));

    if (res != 1)
    {
        printf("Ambient humidity meter set I2C bus errno: %i\n", res);
        return hum;
    }
    else
    {
        uint8_t data[4];
        memset(&data, 0, 4);

        i2c.Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);
        memset(&data, 0, 4);

        res = i2c.Rbb(HIH6130::I2C_ADDRESS, 0x00, data, 4);

        if (res != 4)
        {
            printf("Error while get data for ambient humidity meter.\n");
            return hum;
        }

        hum.status = (data[0] >> 6);

        hum.humidity = (float) (((unsigned int) (data[0] & 0x3F) << 8) | data[1]) * 100 / (pow(2, 14) - 2);

        hum.temperature = (float) (((unsigned int) (data[2] << 6) + (data[3] >> 2)) / (pow(2, 14) - 2) * 165 - 40);

        return hum;
    }
}
