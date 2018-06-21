/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LADC_4_20_H__
#define __LADC_4_20_H__

/*!
    \struct Adc420Data
    \brief Struct with data output for Tibbit 4...20 mA
*/

struct Adc420Data
{
    /// Current in Ampere
    float current;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Adc420
    \brief Class for ADC 4...20 mA (Tibbit #53)
*/

class Adc420
{
public:

    Adc420();

    virtual ~Adc420();

    /// Get current
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param adc Adc420Data data struct
    */
    void getCurrent(const char* socket, Adc420Data &adc);
};

#endif

