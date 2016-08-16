/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LTEMPERATURE_H__
#define __LTEMPERATURE_H__

/*!
    \struct TempData
    \brief Struct with data output for MCP9808
*/

struct TempData
{
    /// Temperature in C degrees
    float temp;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Temperature
    \brief Class for ambient temperature sensor tibbit control (Tibbit #29)
*/

class Temperature
{
public:

    Temperature();

    virtual ~Temperature();

    /// Get current temperature in Celsius degrees
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param temp TempData data struct
    */
    void getTemperature(const char *socket, TempData &temp);
};

#endif
