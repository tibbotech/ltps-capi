/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/portextender.h"

#include "global.h"
#include "lutils.h"

Portextender::Portextender()
{

}

Portextender::~Portextender()
{

}

void Portextender::getData(const char *socket, int pin, PortexData &pextender)
{
    memset(&pextender, 0, sizeof pextender);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        uint8_t data = 0;

        int res = i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);
        pextender.direction = (data >> (pin - 1)) & 1;

        data = 0;

        res += i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
        pextender.pullup = (data >> (pin - 1)) & 1;

        data = 0;

        res += i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
        pextender.value = (data >> (pin - 1)) & 1;

        if (res != 3)
        {
            printf("Checksum error while get data for 8-bit port extender\n");
            return;
        }
    }
    else
        printf("%s\n", error);
}

void Portextender::setData(const char *socket, int pin, PortexData &pextender)
{
    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        uint8_t data = 0;

        int res = i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

        if (pextender.direction)
            data |= (1 << (pin - 1));
        else
            data &= ~(1 << (pin - 1));

        res += i2c->W1b(MCP23008::I2C_ADDRESS, MCP23008::IODIR, data);

        data = 0;

        usleep(10000);

        if (pextender.direction) //< input
        {
            res += i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);

            if (pextender.pullup)
                data |= (1 << (pin - 1));
            else
                data &= ~(1 << (pin - 1));

            res += i2c->W1b(MCP23008::I2C_ADDRESS, MCP23008::GPPU, data);
        }
        else //< output
        {
            res += i2c->R1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);

            if (pextender.value)
                data |= (1 << (pin - 1));
            else
                data &= ~(1 << (pin - 1));

            res += i2c->W1b(MCP23008::I2C_ADDRESS, MCP23008::GPIO, data);
        }

        if (res != 4)
        {
            printf("Checksum error while set data for 8-bit port extender\n");
            return;
        }
    }
    else
        printf("%s\n", error);
}
