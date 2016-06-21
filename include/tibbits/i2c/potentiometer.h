/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPOTENTIOMETER_H__
#define __LPOTENTIOMETER_H__

/*!
    \enum Imps
    \brief Possible impedance maximum values
*/

enum Imps
{
    _5kOhm = 5000, //< #40_1
    _10kOhm = 10000, //< 40_2
    _50kOhm = 50000, //< 40_3
    _100kOhm = 100000 //< 40_4
};

/*!
    \class Potentiometer
    \brief Class for digital potentiometer (Tibbit #40)
*/

class Potentiometer
{
public:

    Potentiometer();

    virtual ~Potentiometer();

    /// Set current impedance and saves the value into the ICs EEPROM
    /*!
        \param socket I2C bus name
        \param impedance Current value
        \param max Maximum posible impedance
    */
    void setImpedance(const char *socket, unsigned int impedance, Imps max);

    /// Set current impedance and saves the value into the ICs EEPROM
    /*!
        \param bus I2C bus number
        \param impedance Current value
        \param max Maximum posible impedance
    */
    void setImpedance(int bus, unsigned int impedance, Imps max);
};

#endif
