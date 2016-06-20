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
        \param bus I2C bus number
        \return Temperature in C degrees
    */
    float getTemperature(int bus);
};

#endif
