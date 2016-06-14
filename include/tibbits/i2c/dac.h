/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __DAC_H__
#define __DAC_H__

#include "drivers/ci2c_smbus.h"
#include "drivers/cpin.h"

/*!
    \class DAC
    \brief Class for DAC (Tibbit #14)
*/

class Dac
{
public:

    /// Constructor
    /*!
        \param socket I2C socket (eg: s1, s11)
    */
    Dac(const char* socket);

    /// Constructor
    /*!
        \param busn I2C bus number
        \param gpin GPIO C-pin
    */
    Dac(uint16_t busn, int gpin_c, int gpin_d);

    virtual ~Dac();

    /// Set voltage for channel
    /*!
        \param channel Integer number of DAC channel (1...4)
        \param voltage Value in millivolts (-10000...10000)
    */
    void setVoltage(unsigned int channel, int voltage);

private:

    Ci2c_smbus m_i2c;

    CPin m_gpio_c, m_gpio_d;

    int m_res;
};

#endif
