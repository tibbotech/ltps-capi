/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/ci2c_smbus.h"
#include "tibbits/i2c/accelerometer.h"

#include "global.h"

Accelerometer::Accelerometer()
{

}

Accelerometer::~Accelerometer()
{

}

Adxl312 Accelerometer::getData(int bus)
{
    Adxl312 accel;
    memset(&accel, 0, sizeof accel);

    Ci2c_smbus i2c;

    int res = i2c.set_bus(bus);

    if (res != 1)
    {
        printf("Accelerometer set I2C bus errno: %i\n", res);
        return accel;
    }

    res = i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::STANDBY_MODE); //< Go into standby mode to configure the device

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_FORMAT, ADXL312::FULL_RES_12G); //< Full resolution, +/-16g, 4mg/LSB

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::MEASUREMENT_MODE); //< Measurement mode

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::FIFO_CTL, ADXL312::BYPASS_MODE); //< Set bypass mode

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::MEASUREMENT_MODE); //< Measurement mode

    usleep(8000);

    int x, y, z;
    uint8_t hi_byte, lo_byte, tmp;

    // Read X axis value

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_X1, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_X1, hi_byte);

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_X0, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_X0, lo_byte);

    x = hi_byte * 256 + lo_byte;

    // Read Y axis value

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y1, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y1, hi_byte);

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y0, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y0, lo_byte);

    y = hi_byte * 256 + lo_byte;

    // Read Z axis value

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z1, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z1, hi_byte);

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z0, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z0, lo_byte);

    z = hi_byte * 256 + lo_byte;

    // In bypass mode the overrun bit is set when new data replaces unread data in the DATAX, DATAY, and DATAZ registers

    res += i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, 0x00, 0);

    res += i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, tmp);

    tmp = tmp & ADXL312::INT_FLAG;

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, tmp);

    res += i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::STANDBY_MODE);

    if (res != 14)
    {
        printf("Error while get data for 3-axis accelerometer\n");
        return accel;
    }

    // Conversions

    int x_value, y_value, z_value;

    if (x & ADXL312::SIGN_MASK)
    {
        x &= ADXL312::DATA_MASK;
        x_value = ADXL312::DATA_MASK - x;
        x_value *= -1;
    }
    else
        x_value = x & ADXL312::DATA_MASK;

    if (y & ADXL312::SIGN_MASK)
    {
        y &= ADXL312::DATA_MASK;
        y_value = ADXL312::DATA_MASK - y;
        y_value *= -1;
     }
     else
        y_value = y & ADXL312::DATA_MASK;

    if (z & ADXL312::SIGN_MASK)
    {
        z &= ADXL312::DATA_MASK;
        z_value = ADXL312::DATA_MASK - z;
        z_value *= -1;
    }
    else
        z_value = z & ADXL312::DATA_MASK;

    x_value *= 2930 / 1000;
    y_value *= 2930 / 1000;
    z_value *= 2930 / 1000;

    accel.lx = x_value;
    accel.ly = y_value;
    accel.lz = z_value;

    return accel;
}
