/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __ADC_H__
#define __ADC_H__

#include "drivers/ci2c_smbus.h"

/*!
    \class ADC
    \brief Class for ADC (Tibbit #13)
*/

class Adc
{
public:

    /// Constructor
    /*!
        \param socket I2C socket (eg: s1, s11)
    */
    Adc(const char* socket);

    /// Constructor
    /*!
        \param busn I2C bus number
    */
    Adc(uint16_t busn);

    virtual ~Adc();

    /// Get voltage value for channel
    /*!
        \param channel ADC channel (1...4)
        \return Value in millivolts
    */
    int getVoltage(unsigned int channel);

private:

    Ci2c_smbus m_i2c;

    int m_res;
};

#endif
