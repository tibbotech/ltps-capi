/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LTEMPERATURE_H__
#define __LTEMPERATURE_H__

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
        \return Temperature in C degrees
    */
    float getTemperature(const char *socket);
};

#endif
