/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPORTEXTENDER_H__
#define __LPORTEXTENDER_H__

/*!
    \struct Mcp23008
    \brief Struct with data for MCP23008
*/

struct Mcp23008
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
        \param socket I2C socket (eg: s1, s11)
        \param pin Current pin (1...8)
        \return Data in Mcp23008 struct
    */
    Mcp23008 getData(const char* socket, uint8_t pin);

    /// Set current data for 8-bit port
    /*!
        \param socket I2C socket (eg: s1, s11)
        \param pin Current pin (1...8)
        \param params Data in Mcp23008 struct
    */
    void setData(const char* socket, uint8_t pin, Mcp23008 params);
};

#endif
