/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LDAC_H__
#define __LDAC_H__

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
        \param bus I2C bus number
        \param gpin_c GPIO C-pin bus number
        \param gpin_d GPIO D-pin bus number
        \param channel Integer number of DAC channel (1...4)
        \param voltage Value in millivolts (-10000...10000)
    */
    void setVoltage(int bus, int gpin_c, int gpin_d, unsigned int channel, int voltage);
};

#endif
