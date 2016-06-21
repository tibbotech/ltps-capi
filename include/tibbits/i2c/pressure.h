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
    float temperature; //< °C
    float pressure; //< mm Hg
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
        \param socket I2C bus name
        \return pres PresData data struct
    */
    void getData(const char *socket, PresData &pres);

    /// Get current pressure/temperature
    /*!
        \param bus I2C bus number
        \return pres PresData data struct
    */
    void getData(int bus, PresData &pres);
};

#endif
