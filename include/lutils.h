/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LUTILS_H__
#define __LUTILS_H__

#include <string>
#include <map>
#include <fstream>

/*!
    \class Lutils
    \brief Help functions for LTPS API
*/

class Lutils
{
public:

    /// Get singleton instance
    /*!
        \return Lutils reference
    */
    static Lutils &getInstance();

    /// Get I2C bus number by LTPS socket
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \return I2C bus number (eg: 0, 4)
    */
    int getI2CBusNum(const char* socket);

    /// Read integer value from from LTPS pins.ini file
    /*!
        \param section Section
        \param param Parameter
        \return Integer value
    */
    int readInteger(const char* section, const char* param);

private:

    Lutils();

    virtual ~Lutils();

    std::ifstream m_fl;

    std::map<std::string, int> m_i2c;
    std::map<std::string, int> m_gpio;

    const char* readString(const char* section, const char* param);

};

#endif
