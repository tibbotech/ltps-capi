/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <string>

/*!
    \namespace Lutilites
    \brief Help functions for LTPS API
*/

namespace Lutilites
{
    /// Read string from INI-file
    /*!
        \param file File name including full path
        \param section Section
        \param param Parameter
        \return String value
    */
    std::string readString(std::string file, std::string section, std::string param);

    /// Read integer from INI-file
    /*!
        \param file File name including full path
        \param section Section
        \param param Parameter
        \return Integer value
    */
    int readInteger(std::string file, std::string section, std::string param);

    /// Convert socket name to I2C bus subname
    /*!
        \param bus String bus name (eg: s3)
        \return I2C bus subname (eg: -s03)
    */
    std::string getI2CName(std::string bus);
}

#endif
