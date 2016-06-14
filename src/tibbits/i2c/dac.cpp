/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/dac.h"

#include "global.h"
#include "utilities.h"

Dac::Dac(const char* socket)
{
    std::string sock(socket);

    std::string hwSocket = Lutilites::readString(PINS_FILE, "I2C", "S" + sock.substr(1, sock.length() - 1));

    if (hwSocket.empty()) //< Software I2C
        m_res = m_i2c.set_bus(Lutilites::getI2CName(sock).c_str());
    else //< Hardware I2C
        m_res = m_i2c.set_bus(atoi(hwSocket.c_str()));

    if (m_res != 1)
        printf("DAC set I2C bus errno: %i\n", m_res);

    int gpin_c = Lutilites::readInteger(PINS_FILE, "CPU", sock.replace(0, 1, "S").append("C").c_str());
    int gpin_d = Lutilites::readInteger(PINS_FILE, "CPU", sock.replace(sock.length() - 1, 1, "D").c_str());

   if ((gpin_c == 0) || (gpin_d == 0))
        printf("DAC GPIO pins search error.\n");
    else
    {
        m_res += m_gpio_c.init(gpin_c);
        m_res += m_gpio_d.init(gpin_d);

        if (m_res != 1)
            printf("DAC GPIO initialization error.\n");
    }
}

Dac::Dac(uint16_t busn, int gpin_c, int gpin_d)
{
    m_res = m_i2c.set_bus(busn);

    if (m_res != 1)
        printf("DAC set I2C bus errno: %i\n", m_res);

    m_res += m_gpio_c.init(gpin_c);
    m_res += m_gpio_d.init(gpin_d);

    if (m_res != 1)
        printf("DAC GPIO initialization error.\n");
}

Dac::~Dac()
{

}

void Dac::setVoltage(unsigned int channel, int voltage)
{
    if (m_res == 1) //< I2C/GPIO initialization successfully
    {
        if ((m_gpio_d.dir_get() == PIN_DIR_O) && (!m_gpio_d.R()))
        {
            printf("Could't to set voltage: DAC is busy.\n");
            return;
        }

        if (m_gpio_c.dir_get() != PIN_DIR_O)
            m_gpio_c.dir_set(PIN_DIR_O); //< LDAC line

        int bvoltage = voltage * 100000;
        bvoltage += 1000000000;
        bvoltage /= 488306;

        uint8_t data[2];

        memset(&data, 0, 3);
        uint8_t reg = MCP4728::MULTI_WRITE + ((channel - 1) % 4) * 2;
        data[0] = MCP4728::VREF_GX + bvoltage / 256;
        data[1] = bvoltage & 0x00FF;

        if (!m_gpio_c.R())
            m_gpio_c.W(1); //< To sure that LDAC is high before set voltages

        m_gpio_c.W(0);

        m_res = m_i2c.Wbb(MCP4728::I2C_ADDRESS, reg, data, 2); //< Multi-write command (see datashit 5.6.2)

        m_gpio_c.W(1);

        if (m_res != 2) //< 2 bytes should be written
        {
            m_res = 1; //< Reset to successfully I2C/GPIO initialization

            printf("Error while set voltage for DAC.\n");
        }
    }
}
