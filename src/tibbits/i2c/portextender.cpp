/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "Ci2c_smbus.h"
#include "tibbits/i2c/portextender.h"

#include "global.h"
#include "lutils.h"

namespace PortextenderPrivate
{
    Ci2c_smbus i2c;
}

Portextender::Portextender()
{

}

Portextender::~Portextender()
{

}

void Portextender::getData(const char *socket, int pin, PortexData &pextender)
{
    int busn = Lutils::getInstance().getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        getData(busn, pin, pextender);
}

void Portextender::getData(int busn, int pin, PortexData &pextender)
{
    memset(&pextender, 0, sizeof pextender);

    int res = PortextenderPrivate::i2c.set_bus(busn);

    if (res != 1)
    {
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data = 0;

    res = PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);
    pextender.direction = (data >> (pin - 1)) & 1;

    data = 0;

    res += PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    pextender.pullup = (data >> (pin - 1)) & 1;

    data = 0;

    res += PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
    pextender.value = (data >> (pin - 1)) & 1;

    if (res != 3)
    {
        printf("Error while get data for 8-bit port extender\n");
        return;
    }
}

void Portextender::setData(const char *socket, int pin, PortexData &pextender)
{
    int busn = Lutils::getInstance().getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        setData(busn, pin, pextender);
}

void Portextender::setData(int busn, int pin, PortexData &pextender)
{
    int res = PortextenderPrivate::i2c.set_bus(busn);

    if (res != 1)
    {
        printf("8-bit port extender set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t data = 0;

    res = PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    if (pextender.direction)
        data |= (1 << (pin - 1));
    else
        data &= ~(1 << (pin - 1));

    res += PortextenderPrivate::i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

    data = 0;

    usleep(10000);

    if (pextender.direction) //< input
    {
        res += PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);

        if (pextender.pullup)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += PortextenderPrivate::i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
    }
    else //< output
    {
        res += PortextenderPrivate::i2c.R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);

        if (pextender.value)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += PortextenderPrivate::i2c.W1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
    }

    if (res != 4)
    {
        printf("Error while set data for 8-bit port extender\n");
        return;
    }
}
