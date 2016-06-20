/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
#include "tibbits/i2c/potentiometer.h"

#include "global.h"
#include "utilities.h"

Potentiometer::Potentiometer()
{

}

Potentiometer::~Potentiometer()
{

}

void Potentiometer::setImpedance(const char *socket, unsigned int impedance, Imps max)
{
    bool result = writeData(socket, MCP4561::VOLATILE_TCON, 0x000F);

    if (!writeData(socket, MCP4561::NON_VOLATILE_WIPER, convertImpToOhm(impedance, max)))
        result = false;

    if (!result)
        printf("Could't to set impedance\n");
}

unsigned int Potentiometer::convertImpToOhm(unsigned int impedance, unsigned int max)
{
    impedance *= 257;
    impedance /= max;

    return impedance;
}

bool Potentiometer::writeData(const char *socket, char addr, unsigned int value)
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
        printf("Digital potentiometer sensor set I2C bus errno: %i\n", res);
        return 0;
    }

    uint8_t cmdByte = 0x00, dataByte = 0x00;

    cmdByte = ((addr << 4) & 0xF0) | (((value & 0x01FF) >> 8) & 0x3);
    dataByte = value & 0x00FF;

    i2c.W1b(MCP4561::I2C_ADDRESS, cmdByte, dataByte);

    usleep(10000);

    if (readData(socket, addr) == value)
        return true;
    else
        return false;
}

unsigned int Potentiometer::readData(const char *socket, char addr)
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
        printf("Digital potentiometer sensor set I2C bus errno: %i\n", res);
        return 0;
    }

    uint8_t data[2];
    memset(&data, 0, sizeof data);

    uint8_t cmdByte, highByte, lowByte;
    cmdByte = (addr << 4) | 0xC;

    i2c.Wbb(MCP4561::I2C_ADDRESS, cmdByte, 0x00, 0);
    i2c.Rbb(MCP4561::I2C_ADDRESS, 0x00, data, 2);

    highByte = data[0];
    lowByte = data[1];

    return (((unsigned int) highByte << 8) | lowByte) & 0x01FF;
}
