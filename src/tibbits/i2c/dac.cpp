/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/dac.h"

#include "global.h"
#include "lutils.h"

#include <string>
#include <algorithm>

Dac::Dac()
{

}

Dac::~Dac()
{

}

void Dac::setVoltage(const char* socket, unsigned int channel, int voltage, DacData &dac)
{
    memset(&dac, 0, sizeof dac);

    std::string sock(socket);
    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    char* error;
    CPin *gpio_c = Lutils::getInstance().getGpioPointer((sock + "C").c_str(), &error);
    if (!gpio_c)
    {
        dac.status = EXIT_FAILURE;
        dac.error = error;
        return;
    }

    CPin *gpio_d = Lutils::getInstance().getGpioPointer((sock + "D").c_str(), &error);
    if (!gpio_d)
    {
        dac.status = EXIT_FAILURE;
        dac.error = error;
        return;
    }

    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);
    if (i2c)
    {
        if ((gpio_d->dir_get() == PIN_DIR_O) && (!gpio_d->R()))
        {
            dac.status = EXIT_FAILURE;
            dac.error = "Could't to set voltage: DAC is busy";
            return;
        }

        if (gpio_c->dir_get() != PIN_DIR_O)
            gpio_c->dir_set(PIN_DIR_O); //< LDAC line

        int bvoltage = voltage * 100000;
        bvoltage += 1000000000;
        bvoltage /= 488306;

        uint8_t data[2];

        memset(&data, 0, 2);
        uint8_t reg = MCP4728::MULTI_WRITE + ((channel - 1) % 4) * 2;
        data[0] = MCP4728::VREF_GX + bvoltage / 256;
        data[1] = bvoltage & 0x00FF;

        if (!gpio_c->R())
            gpio_c->W(1); //< To sure that LDAC is high before set voltages

        gpio_c->W(0);

        int res = i2c->Wbb(MCP4728::I2C_ADDRESS, reg, data, 2); //< Multi-write command (see datashit 5.6.2)

        gpio_c->W(1);

        if (res != 2) //< 2 bytes should be written
        {
            dac.status = EXIT_FAILURE;
            dac.error = "Checksum error while set voltage for DAC";
            return;
        }

        dac.status = EXIT_SUCCESS;
    }
    else
    {
        dac.status = EXIT_FAILURE;
        dac.error = error;
    }
}

void Dac::setVoltage(const char *socket, unsigned int channel, int voltage)
{
    DacData data;
    memset(&data, 0, sizeof data);

    setVoltage(socket, channel, voltage, data);
}
