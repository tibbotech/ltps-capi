/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LTEMPERATURE_H__
#define __LTEMPERATURE_H__


#include "drivers/ci2c_smbus.h"

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
        \param socket I2C socket (eg: s1, s11)
        \return Temperature in C degrees
    */
    float getTemperature(const char* socket);
};

#endif
