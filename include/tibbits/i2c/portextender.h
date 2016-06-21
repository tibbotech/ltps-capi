/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPORTEXTENDER_H__
#define __LPORTEXTENDER_H__

/*!
    \struct PortexData
    \brief Struct with data for MCP23008
*/

struct PortexData
{
    /// Current direction (false for output, true for input)
    bool direction;

    /// Current value (read/write for output, read for input)
    bool value;

    /// Pull-up enabled (read/write for input, read for output)
    bool pullup;
};

/*!
    \class Portextender
    \brief Class for 8-bit port tibbit control (Tibbit #41)
*/

class Portextender
{
public:

    Portextender();

    virtual ~Portextender();

    /// Get current data for 8-bit port
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param pin Current pin (1...8)
        \param pextender Data in PortexData struct
    */
    void getData(const char *socket, int pin, PortexData &pextender);

    /// Get current data for 8-bit port
    /*!
        \param busn I2C bus number (eg: 0, 4)
        \param pin Current pin (1...8)
        \param pextender Data in PortexData struct
    */
    void getData(int busn, int pin, PortexData &pextender);

    /// Set current data for 8-bit port
    /*!
        \param socket I2C bus name (eg: s1, s15)
        \param pin Current pin (1...8)
        \param pextender Data in PortexData struct
    */
    void setData(const char *socket, int pin, PortexData &pextender);

    /// Set current data for 8-bit port
    /*!
        \param busn I2C bus number (eg: 0, 4)
        \param pin Current pin (1...8)
        \param pextender Data in PortexData struct
    */
    void setData(int busn, int pin, PortexData &pextender);
};

#endif
