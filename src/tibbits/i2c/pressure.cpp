/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/pressure.h"

#include "global.h"
#include "lutils.h"

Pressure::Pressure()
{

}

Pressure::~Pressure()
{

}

void Pressure::getData(const char *socket, PresData &pres)
{
    memset(&pres, 0, sizeof pres);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        uint8_t a0_msb, a0_lsb, b1_msb, b1_lsb, b2_msb, b2_lsb, c12_msb, c12_lsb;

        // Reads the calibration coefficients
        int res = i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_A0_MSB, a0_msb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_A0_MSB + 1, a0_lsb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_B1_MSB, b1_msb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_B1_MSB + 1, b1_lsb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_B2_MSB, b2_msb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_B2_MSB + 1, b2_lsb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_C12_MSB, c12_msb);

        res += i2c->R1b(MPL115A2::I2C_ADDRESS, MPL115A2::REG_C12_MSB + 1, c12_lsb);

        // Signs of the coeffs (are correct because we use int16_t ints)
        int16_t a0_, b1_, b2_, c12_;

        // Convert to integers
        a0_ = (a0_msb << 8) + a0_lsb;
        b1_ = (b1_msb << 8) + b1_lsb;
        b2_ = (b2_msb << 8) + b2_lsb;
        c12_ = (c12_msb << 8) + c12_lsb;

        float a0, b1, b2, c12;

        // Convert integers to floats and store them
        a0 = a0_ / 8.0f; //< 2^-3
        b1 = b1_ / 8192.0f; //< 2^-13
        b2 = b2_ / 16384.0f; //< 2^-14
        c12 = c12_ / 16777216.0f; //< 2^-24

        res += i2c->W1b(MPL115A2::I2C_ADDRESS, MPL115A2::CMD_CONVERSION, 0x00);

        usleep(5000);

        uint16_t temperature_word, pressure_word;

        res += i2c->R2b(MPL115A2::I2C_ADDRESS, MPL115A2::TEMPERATURE_MSB, temperature_word);

        res += i2c->R2b(MPL115A2::I2C_ADDRESS, MPL115A2::PRESSURE_MSB, pressure_word);

        if (res != 13)
        {
            pres.status = EXIT_FAILURE;
            pres.error = "Checksum error while get data for barometric pressure sensor";
            return;
        }

        uint8_t pressure_msb, pressure_lsb, temperature_msb, temperature_lsb;

        temperature_msb = (temperature_word & 0x00FF); //< Extract msb byte
        temperature_lsb = (temperature_word & 0xFF00) >> 8;

        pressure_msb = (pressure_word & 0x00FF); //< Extract lsb byte
        pressure_lsb = (pressure_word & 0xFF00) >> 8;

        uint16_t raw_temperature = ((temperature_msb << 8) + temperature_lsb) >> 6;
        uint16_t raw_pressure = ((pressure_msb << 8) + pressure_lsb) >> 6;

        float pressure_comp = a0 + (b1 + c12 * raw_temperature) * raw_pressure + b2 * raw_temperature;

        float temperature, pressure;

        // Black magic formulas: http://forums.adafruit.com/viewtopic.php?f=25&t=34787
        pressure =  ((pressure_comp / 15.737f) + 50.0f) * 7.50062;
        temperature = ((float) raw_temperature) * -0.1707f + 112.27f;

        pres.pressure = pressure;
        pres.temperature = temperature;

        pres.status = EXIT_SUCCESS;
    }
    else
    {
        pres.status = EXIT_FAILURE;
        pres.error = error;
    }
}
