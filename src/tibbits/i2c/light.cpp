/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/light.h"

#include "global.h"
#include "utilities.h"

Light::Light()
{

}

Light::~Light()
{

}

int Light::getIllumination(const char* socket)
{
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
        printf("Ambient light sensor set I2C bus errno: %i\n", res);
        return 0;
    }

    uint8_t data[2];
    memset(&data, 0, 2);

    res = i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWEROFF, data, 0);

    res += i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::POWERON, data, 0);

    res += i2c.Wbb(BH1721FVC::I2C_ADDRESS, BH1721FVC::HIGHRES, data, 0);

    usleep(180000);

    res += i2c.Rbb(BH1721FVC::I2C_ADDRESS, 0x00, data, 2);

    if (res != 2)
    {
        printf("Error while get illumination for ambient light sensor\n");
        return 0;
    }

    return (data[0] * 256 + data[1]) / 1.2;
}
