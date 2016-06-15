/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/dac.h"

#include "global.h"
#include "utilities.h"

Dac::Dac()
{

}

Dac::~Dac()
{

}

void Dac::setVoltage(const char* socket, unsigned int channel, int voltage)
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
        printf("DAC set I2C bus errno: %i\n", res);
        return;
    }

    int gpin_c = Lutilites::readInteger(PINS_FILE, "CPU", sock.replace(0, 1, "S").append("C").c_str());
    int gpin_d = Lutilites::readInteger(PINS_FILE, "CPU", sock.replace(sock.length() - 1, 1, "D").c_str());

    if ((gpin_c == 0) || (gpin_d == 0))
    {
        printf("DAC GPIO pins search error\n");
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
