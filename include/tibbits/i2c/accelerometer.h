/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include "drivers/ci2c_smbus.h"

/*!
    \struct Adxl312
    \brief Struct with data output for ADXL312
*/

struct Adxl312
{
    int lx; //< mG
    int ly; //< mG
    int lz; //< mG
};

/*!
    \class Accelerometer
    \brief Class for 3-axis accelerometer (Tibbit #36)
*/

class Accelerometer
{
public:

    Accelerometer();

    virtual ~Accelerometer();

    /// Get current data for accelerometer axises
    /*!
        \param socket I2C socket (eg: s1, s11)
        \return Adxl312 data struct
    */
    Adxl312 getData(const char* socket);
};

#endif
