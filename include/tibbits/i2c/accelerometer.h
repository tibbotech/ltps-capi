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

    /// Constructor
    /*!
        \param socket I2C socket (eg: -s01, -s11)
    */
    Accelerometer(const char* socket);

    /// Constructor
    /*!
        \param busn I2C bus number
    */
    Accelerometer(uint16_t busn);

    virtual ~Accelerometer();

    /// Get current data for accelerometer axises
    /*!
        \return Adxl312 data struct
    */
    Adxl312 getData();

private:

    Ci2c_smbus m_i2c;

    int m_res;
};

#endif
