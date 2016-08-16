/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LADC_H__
#define __LADC_H__

/*!
    \struct AdcData
    \brief Struct with data output for LTC2309
*/

struct AdcData
{
    /// Voltage in millivolts
    int voltage;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Adc
    \brief Class for ADC (Tibbit #13)
*/

class Adc
{
public:

    Adc();

    virtual ~Adc();

    /// Get voltage value for channel
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param channel ADC channel (1...4)
        \param prev Get previous voltage conversion for true
        \param adc AdcData data struct
    */
    void getVoltage(const char* socket, unsigned int channel, bool prev, AdcData &adc);
};

#endif
