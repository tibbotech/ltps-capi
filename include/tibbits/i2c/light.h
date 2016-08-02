/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LLIGHT_H__
#define __LLIGHT_H__

/*!
    \class Light
    \brief Class for ambient light sensor tibbit control (Tibbit #28)
*/

class Light
{
public:

    Light();

    virtual ~Light();

    /// Get current illumination in lx
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \return Illumination in lx
    */
    int getIllumination(const char *socket);
};

#endif
