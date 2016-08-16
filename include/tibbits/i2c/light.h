/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LLIGHT_H__
#define __LLIGHT_H__

/*!
    \struct LightData
    \brief Struct with data output for BH1721FVC
*/

struct LightData
{
    /// Illumination in lx
    int ill;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \class Light
    \brief Class for ambient light sensor tibbit control (Tibbit #28)
*/

class Light
{
public:

    Light();

    virtual ~Light();

    /// Get current illumination
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param data LightData data struct
    */
    void getIllumination(const char *socket, LightData &ill);
};

#endif
