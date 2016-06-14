/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LHUMIDITY_H__
#define __LHUMIDITY_H__

#include "drivers/ci2c_smbus.h"

/*!
    \struct Hih6130
    \brief Struct with data output for HIH6130
*/

struct Hih6130
{
    float humidity; //< % RH
    float temperature; //< °C
    uint8_t status;
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
        \param socket I2C socket (eg: s1, s11)
        \return Hih6130 data struct
    */
    Hih6130 getData(const char* socket);
};

#endif
