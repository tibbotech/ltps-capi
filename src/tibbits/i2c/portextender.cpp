/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/portextender.h"

#include "global.h"
#include "utilities.h"

Portextender::Portextender()
{

}

Portextender::~Portextender()
{

}

Mcp23008 Portextender::getData(const char *socket, uint8_t pin)
{
    Mcp23008 params;
    memset(&params, 0, sizeof params);

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
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return params;
    }

    uint8_t data = 0;

    res = i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);
    params.direction = (data >> (pin - 1)) & 1;

    data = 0;

    res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    params.pullup = (data >> (pin - 1)) & 1;

    data = 0;

    res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
    params.value = (data >> (pin - 1)) & 1;


    if (res != 3)
    {
        printf("Error while get data for 8-bit port extender\n");
        return params;
    }

    return params;
}

void Portextender::setData(const char *socket, uint8_t pin, Mcp23008 params)
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
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data = 0;

    res = i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    if (params.direction)
        data |= (1 << (pin - 1));
    else
        data &= ~(1 << (pin - 1));

    res += i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    data = 0;

    usleep(10000);

    if (params.direction) //< input
    {
        res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);

        if (params.pullup)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    }
    else //< output
    {
        res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);

        if (params.value)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
    }

    if (res != 4)
    {
        printf("Error while set data for 8-bit port extender\n");
        return;
    }
}
