/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPRESSURE_H__
#define __LPRESSURE_H__

#include "drivers/ci2c_smbus.h"

/*!
    \struct Mpl115a2
    \brief Struct with data output for MPL115A2
*/

struct Mpl115a2
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
        \param socket I2C socket (eg: s1, s11)
        \return Mpl115a2 data struct
    */
    Mpl115a2 getData(const char* socket);
};

#endif
