/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LADC_H__
#define __LADC_H__

/*!
    \class ADC
    \brief Class for ADC (Tibbit #13)
*/

class Adc
{
public:

    Adc();

    virtual ~Adc();

    /// Get voltage value for channel
    /*!
        \param socket I2C socket (eg: s1, s11)
        \param channel ADC channel (1...4)
        \return Value in millivolts
    */
    int getVoltage(const char* socket, unsigned int channel);
};

#endif
