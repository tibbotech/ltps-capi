/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
#include "drivers/cpin.h"
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

void Dac::setVoltage(const char* socket, unsigned int channel, int voltage)
{
    std::string sock(socket);
    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    int busn = Lutils::getI2CBusNum(socket);
    int gpin_c = Lutils::readInteger(PINS_FILE, "CPU", std::string(sock + "C").c_str());
    int gpin_d = Lutils::readInteger(PINS_FILE, "CPU", std::string(sock + "D").c_str());

    if (busn == -1)
    {
        printf("I2C bus for socket %s not found\n", socket);
        return;
    }

    if ((gpin_c == 0) || (gpin_d == 0))
    {
        printf("GPIO pins for socket %s not found\n", socket);
        return;
    }

    setVoltage(busn, gpin_c, gpin_d, channel, voltage);
}

void Dac::setVoltage(int busn, int gpin_c, int gpin_d, unsigned int channel, int voltage)
{
    Ci2c_smbus i2c;

    int res = i2c.set_bus(busn);

    if (res != 1)
    {
        printf("DAC set I2C bus errno: %i\n", res);
        return;
    }

    CPin gpio_c, gpio_d;

    res += gpio_c.init(gpin_c);
    res += gpio_d.init(gpin_d);

    if (res != 1)
    {
        printf("DAC GPIO initialization error\n");
        return;
    }

    if ((gpio_d.dir_get() == PIN_DIR_O) && (!gpio_d.R()))
    {
        printf("Could't to set voltage: DAC is busy\n");
        return;
    }

    if (gpio_c.dir_get() != PIN_DIR_O)
        gpio_c.dir_set(PIN_DIR_O); //< LDAC line

    int bvoltage = voltage * 100000;
    bvoltage += 1000000000;
    bvoltage /= 488306;

    uint8_t data[2];

    memset(&data, 0, 3);
    uint8_t reg = MCP4728::MULTI_WRITE + ((channel - 1) % 4) * 2;
    data[0] = MCP4728::VREF_GX + bvoltage / 256;
    data[1] = bvoltage & 0x00FF;

    if (!gpio_c.R())
        gpio_c.W(1); //< To sure that LDAC is high before set voltages

    gpio_c.W(0);

    res = i2c.Wbb(MCP4728::I2C_ADDRESS, reg, data, 2); //< Multi-write command (see datashit 5.6.2)

    gpio_c.W(1);

    if (res != 2) //< 2 bytes should be written
        printf("Error while set voltage for DAC\n");
}
