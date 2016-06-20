/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
#include "tibbits/i2c/adc.h"

#include "global.h"
#include "utilities.h"

Adc::Adc()
{

}

Adc::~Adc()
{

}

int Adc::getVoltage(const char* socket, unsigned int channel)
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
        printf("ADC set I2C bus errno: %i\n", res);
        return 0;
    }

    unsigned int addr = LTC2309::CH0 | LTC2309::UNIPOLAR_MODE; //< Channel 1

    if (channel == 2)
        addr = LTC2309::CH2 | LTC2309::UNIPOLAR_MODE;
    if (channel == 3)
        addr = LTC2309::CH4 | LTC2309::UNIPOLAR_MODE;
    if (channel == 4)
        addr = LTC2309::CH6 | LTC2309::UNIPOLAR_MODE;

    uint8_t data[2];
    memset(&data, 0, 2);

    i2c.Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Throws out last reading
    memset(&data, 0, 2); // Clear last reading result

    res = i2c.Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Obtains the current reading and stores to data variable

    if (res != 2)
    {
        printf("Error while get voltage for ADC\n");
        return 0;
    }

    // Сonverting to mV
    int result = data[1] / 64 + data[0] * 4;
    result = (result * 1953223 - 1000000000) / 100000;

    return result;
}
