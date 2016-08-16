/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LUTILS_H__
#define __LUTILS_H__

#include <map>
#include <fstream>
#include <cstring>

#include "Ci2c_smbus.h"
#include "cpin.h"

/*!
    \struct CompareCStrings
    \brief Comparator for C-strings
*/

struct CompareCStrings
{
    bool operator() (const char* lhs, const char* rhs) const
    {
        return std::strcmp(lhs, rhs) < 0;
    }
};

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
    int getI2CBusNum(const char* ocket);

    /// Read integer value from from LTPS pins.ini file
    /*!
        \param section Section
        \param param Parameter
        \return Integer value
    */
    int readInteger(const char* section, const char* param);

    /// Get CPin pointer
    /*!
        \param pin Pin name (eg: S1A, S15B)
        \param error Return string error if something goes wrong
        \return CPin pointer
    */
    CPin* getGpioPointer(const char* pin, char **error);

    /// Get Ci2c_smbus pointer
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param error Return string error if something goes wrong
        \return Ci2c_smbus pointer
    */
    Ci2c_smbus* getI2CPointer(const char* socket, char **error);

private:

    Lutils();

    virtual ~Lutils();

    Lutils(const Lutils&);

    Lutils &operator =(const Lutils &);

    std::ifstream m_fl;

    std::map<const char*, int, CompareCStrings> m_si2c;

    std::map<const char*, int, CompareCStrings> m_sgpio;

    std::map<const char*, CPin*, CompareCStrings> m_pins;

    std::map<const char*, Ci2c_smbus*, CompareCStrings> m_i2c;

    const char* readString(const char* section, const char* param);
};

#endif
