/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/accelerometer.h"

#include "global.h"
#include "utilities.h"

Accelerometer::Accelerometer(const char* socket)
{
    std::string sock(socket);

    std::string hwSocket = Lutilites::readString(PINS_FILE, "I2C", "S" + sock.substr(1, sock.length() - 1));

    if (hwSocket.empty()) //< Software I2C
        m_res = m_i2c.set_bus(Lutilites::getI2CName(sock).c_str());
    else //< Hardware I2C
        m_res = m_i2c.set_bus(atoi(hwSocket.c_str()));

    if (m_res != 1)
        printf("Accelerometer set I2C bus errno: %i\n", m_res);
}

Accelerometer::Accelerometer(uint16_t busn)
{
    m_res = m_i2c.set_bus(busn);

    if (m_res != 1)
        printf("Accelerometer set I2C bus errno: %i\n", m_res);
}

Accelerometer::~Accelerometer()
{

}

Adxl312 Accelerometer::getData()
{
    Adxl312 accel;
    memset(&accel, 0, sizeof accel);

    if (m_res == 1) //< I2C setbus successfully
    {
        m_res = m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::STANDBY_MODE); //< Go into standby mode to configure the device

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_FORMAT, ADXL312::FULL_RES_12G); //< Full resolution, +/-16g, 4mg/LSB

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::MEASUREMENT_MODE); //< Measurement mode

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::FIFO_CTL, ADXL312::BYPASS_MODE); //< Set bypass mode

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::MEASUREMENT_MODE); //< Measurement mode

        usleep(8000);

        int x, y, z;
        uint8_t hi_byte, lo_byte, tmp;

        // Read X axis value

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_X1, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_X1, hi_byte);

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_X0, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_X0, lo_byte);

        x = hi_byte * 256 + lo_byte;

        // Read Y axis value

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y1, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y1, hi_byte);

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y0, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Y0, lo_byte);

        y = hi_byte * 256 + lo_byte;

        // Read Z axis value

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z1, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z1, hi_byte);

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z0, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::DATA_Z0, lo_byte);

        z = hi_byte * 256 + lo_byte;

        // In bypass mode the overrun bit is set when new data replaces unread data in the DATAX, DATAY, and DATAZ registers

        m_res += m_i2c.Wbb(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, 0x00, 0);

        m_res += m_i2c.R1b(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, tmp);

        tmp = tmp & ADXL312::INT_FLAG;

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::INT_ENABLE, tmp);

        m_res += m_i2c.W1b(ADXL312::I2C_ADDRESS, ADXL312::POWER_CTL, ADXL312::STANDBY_MODE);

        if (m_res != 14)
        {
            m_res = 1; //< Reset to successfully I2C setbus

            printf("Error while get data for 3-axis accelerometer.\n");

            return accel;
        }
        else
            m_res = 1; //< Reset to successfully I2C setbus

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
    }

    return accel;
}
