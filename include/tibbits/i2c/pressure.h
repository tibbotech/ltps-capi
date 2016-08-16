/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPRESSURE_H__
#define __LPRESSURE_H__

/*!
    \struct PresData
    \brief Struct with data output for MPL115A2
*/

struct PresData
{
    /// Temperature in degrees Celsius
    float temperature;

    /// Pressure in mm Hg
    float pressure;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Pressure
    \brief Class for ambient pressure/temperature sensor tibbit control (Tibbit #35)
*/

class Pressure
{
public:

    Pressure();

    virtual ~Pressure();

    /// Get current pressure/temperature
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param pres PresData data struct
    */
    void getData(const char *socket, PresData &pres);
};

#endif
