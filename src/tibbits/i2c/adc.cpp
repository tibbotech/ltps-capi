/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/adc.h"

#include "global.h"
#include "utilities.h"

Adc::Adc(const char* socket)
{
    std::string sock(socket);

    std::string hwSocket = Lutilites::readString(PINS_FILE, "I2C", "S" + sock.substr(1, sock.length() - 1));

    if (hwSocket.empty()) //< Software I2C
        m_res = m_i2c.set_bus(Lutilites::getI2CName(sock).c_str());
    else //< Hardware I2C
        m_res = m_i2c.set_bus(atoi(hwSocket.c_str()));

    if (m_res != 1)
        printf("ADC set I2C bus errno: %i\n", m_res);
}

Adc::Adc(uint16_t busn)
{
    m_res = m_i2c.set_bus(busn);

    if (m_res != 1)
        printf("ADC set I2C bus errno: %i\n", m_res);
}

Adc::~Adc()
{

}

int Adc::getVoltage(unsigned int channel)
{
    int result = 0;

    if (m_res == 1)  //< I2C setbus successfully
    {
        unsigned int addr = LTC2309::CH0 | LTC2309::UNIPOLAR_MODE; //< Channel 1

        if (channel == 2)
            addr = LTC2309::CH2 | LTC2309::UNIPOLAR_MODE;
        if (channel == 3)
            addr = LTC2309::CH4 | LTC2309::UNIPOLAR_MODE;
        if (channel == 4)
            addr = LTC2309::CH6 | LTC2309::UNIPOLAR_MODE;

        uint8_t data[2];
        memset(&data, 0, 2);

        m_i2c.Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Throws out last reading
        memset(&data, 0, 2); // Clear last reading result

        m_res = m_i2c.Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Obtains the current reading and stores to data variable

        if (m_res != 2)
        {
            m_res = 1; //< Reset to successfully I2C setbus

            printf("Error while get voltage for ADC.\n");

            return 0;
        }
        else
            m_res = 1; //< Reset to successfully I2C setbus

        // Сonverting to mV
        result = data[1] / 64 + data[0] * 4;
        result = (result * 1953223 - 1000000000) / 100000;
    }

    return result;
}
