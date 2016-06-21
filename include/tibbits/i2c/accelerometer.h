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
        \param socket I2C bus name
        \return accel AccelData data struct
    */
    void getData(const char* socket, AccelData &accel);

    /// Get current data for accelerometer axises
    /*!
        \param busn I2C bus number
        \return accel AccelData data struct
    */
    void getData(int busn, AccelData &accel);
};

#endif
