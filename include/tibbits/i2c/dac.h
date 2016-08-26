/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LDAC_H__
#define __LDAC_H__

/*!
    \struct DacData
    \brief Struct with data output for MCP4728
*/

struct DacData
{
    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Dac
    \brief Class for DAC (Tibbit #14)
*/

class Dac
{
public:

    Dac();

    virtual ~Dac();

    /// Set voltage for channel
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param channel Integer number of DAC channel (1...4)
        \param voltage Value in millivolts (-10000...10000)
        \param dac DacData data struct
    */
    void setVoltage(const char* socket, unsigned int channel, int voltage, DacData &dac);

    /// Set voltage for channel
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param channel Integer number of DAC channel (1...4)
        \param voltage Value in millivolts (-10000...10000)
    */
    void setVoltage(const char* socket, unsigned int channel, int voltage);
};

#endif
