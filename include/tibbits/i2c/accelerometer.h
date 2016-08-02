/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LACCELEROMETER_H__
#define __LACCELEROMETER_H__

/*!
    \struct AccelData
    \brief Struct with data output for ADXL312
*/

struct AccelData
{
    /// Acceleration for X-axis im mG
    int lx;

    /// Acceleration for Y-axis im mG
    int ly;

    /// Acceleration for Z-axis im mG
    int lz;
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
        \param socket I2C bus name (eg: s1, s15)
        \param accel AccelData data struct
    */
    void getData(const char* socket, AccelData &accel);
};

#endif
