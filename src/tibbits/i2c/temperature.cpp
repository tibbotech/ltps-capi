/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/temperature.h"

#include "global.h"
#include "lutils.h"

Temperature::Temperature()
{

}

Temperature::~Temperature()
{

}

float Temperature::getTemperature(const char *socket)
{
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket);

    if (i2c)
    {
        int res = i2c->W1b(MCP9808::I2C_ADDRESS, MCP9808::RESOLUTION, MCP9808::RESOLUSIONMODE); //< 0.25C resolution

        usleep(65000);

        uint16_t data;

        res += i2c->R2b(MCP9808::I2C_ADDRESS, MCP9808::TA, data);

        if (res != 3)
        {
            printf("Error while get temperature for ambient temperature sensor\n");
            return 0;
        }

        // Swap order of the msb and lsb bytes
        uint16_t raw_temperature = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);

        float temperature = (raw_temperature & 0x0FFF) / 16.0;

        if (raw_temperature & 0x1000) //< Check sign bit
            temperature -= 256.0;

        return temperature;
    }
    else
        printf("Error while get I2C bus for ambient temperature sensor\n");

    return 0.0f;
}
