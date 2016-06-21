/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPORTEXTENDER_H__
#define __LPORTEXTENDER_H__

/*!
    \struct ExtenderData
    \brief Struct with data for MCP23008
*/

struct ExtenderData
{
    bool direction; //< false - output, true - input
    bool value; //< read/write for output, read for input
    bool pullup; //< read/write for input, read for output
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
        \param socket I2C bus name
        \param pin Current pin (1...8)
        \return extender Data in ExtenderData struct
    */
    void getData(const char *socket, int pin, ExtenderData &extender);

    /// Get current data for 8-bit port
    /*!
        \param busn I2C bus number
        \param pin Current pin (1...8)
        \return extender Data in ExtenderData struct
    */
    void getData(int busn, int pin, ExtenderData &extender);

    /// Set current data for 8-bit port
    /*!
        \param socket I2C bus name
        \param pin Current pin (1...8)
        \param params Data in ExtenderData struct
    */
    void setData(const char *socket, int pin, ExtenderData &extender);

    /// Set current data for 8-bit port
    /*!
        \param busn I2C bus number
        \param pin Current pin (1...8)
        \param params Data in ExtenderData struct
    */
    void setData(int busn, int pin, ExtenderData &extender);
};

#endif
