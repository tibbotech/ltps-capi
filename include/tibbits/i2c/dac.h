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

    Dac();

    virtual ~Dac();

    /// Set voltage for channel
    /*!
        \param socket I2C socket (eg: s1, s11)
        \param channel Integer number of DAC channel (1...4)
        \param voltage Value in millivolts (-10000...10000)
    */
    void setVoltage(const char* socket, unsigned int channel, int voltage);
};

#endif
