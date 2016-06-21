/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
#include "tibbits/i2c/portextender.h"

#include "global.h"
#include "lutils.h"

Portextender::Portextender()
{

}

Portextender::~Portextender()
{

}

void Portextender::getData(const char *socket, int pin, ExtenderData &extender)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        getData(busn, pin, extender);
}

void Portextender::getData(int busn, int pin, ExtenderData &extender)
{
    memset(&extender, 0, sizeof extender);

    Ci2c_smbus i2c;

    int res = i2c.set_bus(busn);

    if (res != 1)
    {
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data = 0;

    res = i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);
    extender.direction = (data >> (pin - 1)) & 1;

    data = 0;

    res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    extender.pullup = (data >> (pin - 1)) & 1;

    data = 0;

    res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
    extender.value = (data >> (pin - 1)) & 1;

    if (res != 3)
    {
        printf("Error while get data for 8-bit port extender\n");
        return;
    }
}

void Portextender::setData(const char *socket, int pin, ExtenderData &extender)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        setData(busn, pin, extender);
}

void Portextender::setData(int busn, int pin, ExtenderData &extender)
{
    Ci2c_smbus i2c;

    int res = i2c.set_bus(busn);

    if (res != 1)
    {
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data = 0;

    res = i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    if (extender.direction)
        data |= (1 << (pin - 1));
    else
        data &= ~(1 << (pin - 1));

    res += i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    data = 0;

    usleep(10000);

    if (extender.direction) //< input
    {
        res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);

        if (extender.pullup)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    }
    else //< output
    {
        res += i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);

        if (extender.value)
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
