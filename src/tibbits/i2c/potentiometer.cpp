/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/potentiometer.h"

#include "global.h"
#include "lutils.h"

namespace PotentiometerPrivate
{
    unsigned int convertImpToOhm(unsigned int impedance, unsigned int max)
    {
        impedance *= 257;
        impedance /= max;

        return impedance;
    }

    unsigned int readData(const char* socket, char addr)
    {
        char* error;
        Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

        if (i2c)
        {
            uint8_t data[2];
            memset(&data, 0, sizeof data);

            uint8_t cmdByte, highByte, lowByte;
            cmdByte = (addr << 4) | 0xC;

            i2c->Wbb(MCP4561::I2C_ADDRESS, cmdByte, 0x00, 0);
            i2c->Rbb(MCP4561::I2C_ADDRESS, 0x00, data, 2);

            highByte = data[0];
            lowByte = data[1];

            return (((unsigned int) highByte << 8) | lowByte) & 0x01FF;
        }
        else
            printf("%s\n", error);

        return 0;
    }

    bool writeData(const char* socket, char addr, unsigned int value)
    {
        char* error;
        Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

        if (i2c)
        {
            uint8_t cmdByte = 0x00, dataByte = 0x00;

            cmdByte = ((addr << 4) & 0xF0) | (((value & 0x01FF) >> 8) & 0x3);
            dataByte = value & 0x00FF;

            i2c->W1b(MCP4561::I2C_ADDRESS, cmdByte, dataByte);

            usleep(10000);

            if (readData(socket, addr) == value)
                return true;
            else
                return false;
        }
        else
            printf("%s\n", error);

        return false;
    }
}

Potentiometer::Potentiometer()
{

}

Potentiometer::~Potentiometer()
{

}

void Potentiometer::setImpedance(const char *socket, unsigned int impedance, Imps max)
{
    bool result = PotentiometerPrivate::writeData(socket, MCP4561::VOLATILE_TCON, 0x000F);

    if (!PotentiometerPrivate::writeData(socket, MCP4561::NON_VOLATILE_WIPER, PotentiometerPrivate::convertImpToOhm(impedance, max)))
        result = false;

    if (!result)
        printf("Could't to set impedance\n");
}
