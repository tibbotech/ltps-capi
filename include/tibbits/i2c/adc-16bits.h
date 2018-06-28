/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LADC_16_H__
#define __LADC_16_H__

/*!
    \struct Adc16Data
    \brief Struct with data output for ADC 16bits
*/

struct Adc16Data
{
    /// Voltage in Volts
    float voltage;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Adc16
    \brief Class for ADC 16bits tibbit control (Tibbit #52)
*/

class Adc16
{
public:

    Adc16();

    virtual ~Adc16();

    /// Get current voltage
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param channel ADC channel (1...4)
        \param adc Adc16Data data struct
    */
    void getVoltage(const char* socket, unsigned int channel, Adc16Data &adc);
};

#endif
