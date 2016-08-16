/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LHUMIDITY_H__
#define __LHUMIDITY_H__

/*!
    \struct HumData
    \brief Struct with data output for HIH6130
*/

struct HumData
{
    /// RH in percents
    float humidity;

    /// Temperature in degrees Celsius
    float temperature;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Humidity
    \brief Class for ambient humidity/temperature meter tibbit control (Tibbit #30)
*/

class Humidity
{
public:

    Humidity();

    virtual ~Humidity();

    /// Get current humidity/temperature
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param hum HumData data struct
    */
    void getData(const char* socket, HumData &hum);
};

#endif
