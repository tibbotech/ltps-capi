/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LUTILS_H__
#define __LUTILS_H__

#define PINS_INI_FILE           "/opt/tps-shared/hwini/pins.ini"

/*!
    \namespace Lutils
    \brief Help functions for LTPS API
*/

namespace Lutils
{
    /// Get I2C bus number by socket
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \return I2C bus number (eg: 0, 4)
    */
    int getI2CBusNum(const char* socket);

    /// Read string from INI-file
    /*!
        \param file File name including full path
        \param section Section
        \param param Parameter
        \return String value
    */
    const char* readString(const char* file, const char* section, const char* param);

    /// Read integer from INI-file
    /*!
        \param file File name including full path
        \param section Section
        \param param Parameter
        \return Integer value
    */
    int readInteger(const char* file, const char* section, const char* param);
}

#endif
