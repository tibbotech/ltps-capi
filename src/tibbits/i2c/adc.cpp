/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/adc.h"

#include "global.h"
#include "lutils.h"

Adc::Adc()
{

}

Adc::~Adc()
{

}

void Adc::getVoltage(const char* socket, unsigned int channel, bool prev, AdcData &adc)
{
    memset(&adc, 0, sizeof adc);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
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

        if (!prev)
        {
            i2c->Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Throws out last reading
            memset(&data, 0, 2); // Clear last reading result
        }

        int res = i2c->Rbb(LTC2309::I2C_ADDRESS, addr, data, 2); //< Obtains the current reading and stores to data variable

        if (res != 2)
        {
            adc.status = EXIT_FAILURE;
            adc.error = "Checksum error while get voltage for ADC";
            return;
        }

        // Сonverting to mV
        int result = data[1] / 64 + data[0] * 4;
        result = (result * 1953223 - 1000000000) / 100000;

        adc.voltage = result;

        adc.status = EXIT_SUCCESS;
    }
    else
    {
        adc.status = EXIT_FAILURE;
        adc.error = error;
    }
}
