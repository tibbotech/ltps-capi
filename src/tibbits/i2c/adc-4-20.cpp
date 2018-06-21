/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/adc-4-20.h"

#include "global.h"
#include "lutils.h"

Adc420::Adc420()
{

}

Adc420::~Adc420()
{

}

void Adc420::getCurrent(const char* socket, Adc420Data &adc)
{
    memset(&adc, 0, sizeof adc);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        const float lsbV = 5 / (float) 323768;
        const float mAonV = (20 - 4) / (float) 5000;

        float rtn = 0.0f;

        uint16_t data;
        int res = i2c->R2b(ADS1100R::I2C_ADDRESS, 0x00, data);

        if (res != 2)
        {
            adc.status = EXIT_FAILURE;
            adc.error = "Checksum error while get current for ADC";
            return;
        }

        // Swap order of the msb and lsb bytes
        uint16_t raw_data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8);

        if (raw_data <= 0x7FFF)
            rtn = raw_data * lsbV;
        else
        {
            rtn = 0xFFFF - raw_data + 1;
            rtn = -(rtn * lsbV);
        }

        float result = 0.0f;
        if (rtn < -0.6f)
            result = 0.0f;
        else
        {
            float ma = rtn * mAonV;
            ma += 0.004;
            result = ma;
        }

        adc.current = result;

        adc.status = EXIT_SUCCESS;
    }
    else
    {
        adc.status = EXIT_FAILURE;
        adc.error = error;
    }
}

