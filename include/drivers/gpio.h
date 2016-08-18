/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LGPIO_H__
#define __LGPIO_H__

/*!
    \class Gpio
    \brief Class for GPIO manipulations
*/

/*!
    Bit arithmetic

    1. To disable n bit in data
    data &= ~(1 << n)
    2.To enable n bit in data
    data |= (1 << n)
    3. To toogle n bit in data
    data ^= (1 << n)
    4. To get n nit in data
    (data >> n) & 1
*/


class Gpio
{
public:

    Gpio();

    virtual ~Gpio();

    /// Set direction
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \param direction Pin direction (0 for input, 1 for output)
        \return Result (0 on success)
    */
    int setDirection(const char *pin, int direction);

    /// Set directions for the pin group simultaneously (see bit arithmetic)
    /*!
        \param lsbPins Pins S1A...S15D (LSB bit 0 - S1A, bit 1 - S1B,.., MSB bit 31 - S15D)
        \param msbPins Pins S17A..S25C (LSB bit 0 - S17A, bit 1 - S17B,..., MSB bit 18 - S25D)
        \param lsbDirs Directions for pins S1A...S15D (0 for input, 1 for output)
        \param msbDirs Directions for pins S17A...S25D (0 for input, 1 for output)
    */
    void setDirection(int lsbPins, int msbPins, int lsbDirs, int msbDirs);

    /// Get direction
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin direction (0 for input, 1 for output)
    */
    int getDirection(const char *pin);

    /// Set value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \param value Pin value (0 or 1)
        \return Result (< 0 on error)
    */
    int setValue(const char* pin, unsigned int value);

    /// Set values for the pin group simultaneously (see bit arithmetic)
    /*!
        \param lsbPins Pins S1A...S15D (LSB bit 0 - S1A, bit 1 - S1B,.., MSB bit 31 - S15D)
        \param msbPins Pins S17A..S25C (LSB bit 0 - S17A, bit 1 - S17B,..., MSB bit 18 - S25D)
        \param lsbValues Directions for pins S1A...S15D (0 for input, 1 for output)
        \param msbValues Directions for pins S17A...S25D (0 for input, 1 for output)
    */
    void setValue(int lsbPins, int msbPins, int lsbValues, int msbValues);

    /// Get value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin value (0 or 1)
    */
    unsigned int getValue(const char *pin);

    /// Get CPU pin number
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return CPU pin number
    */
    unsigned int getPinNumber(const char* pin);
};

#endif
