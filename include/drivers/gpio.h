/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LGPIO_H__
#define __LGPIO_H__

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

/*!
    \struct GpioReturn
    \brief Structure for return values
*/

struct GpioReturn
{
    int msb;
    int lsb;
};

/*!
    \class Gpio
    \brief Class for GPIO manipulations
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

    /// Set directions for all pins simultaneously (see bit arithmetic)
    /*!
        \param lsbPins Applicable bits: pins S1A...S15D (LSB bit 0 - pin S1A, bit 1 - pin S1B,.., MSB bit 31 - pin S15D)
        \param msbPins Applicable bits: pins S17A..S25C (LSB bit 0 - pin S17A, bit 1 - pin S17B,..., MSB bit 18 - pin S25D)
        \param lsbDirs Directions for first group: pins S1A...S15D (0 for input, 1 for output)
        \param msbDirs Directions for second group: pins S17A...S25D (0 for input, 1 for output)
    */
    void setDirections(int lsbPins, int msbPins, int lsbDirs, int msbDirs);

    /// Get direction
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin direction (0 for input, 1 for output)
    */
    int getDirection(const char *pin);

    /// Get directions for all pins simultaneously (see bit arithmetic)
    /*!
        \param lsbDirs Directions for first group: pins S1A...S15D (0 for input, 1 for output)
        \param msbDirs Directions for second group: pins S17A...S25D (0 for input, 1 for output)
    */
    void getDirections(int &lsbDirs, int &msbDirs);

    /// Get directions for all pins simultaneously
    /*!
        \return GpioReturn structure
    */
    GpioReturn getDirections();

    /// Set value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \param value Pin value (0 or 1)
        \return Result (< 0 on error)
    */
    int setValue(const char* pin, unsigned int value);

    /// Set values for all pins simultaneously (see bit arithmetic)
    /*!
        \param lsbPins Applicable bits: pins S1A...S15D (LSB bit 0 - pin S1A, bit 1 - pin S1B,.., MSB bit 31 - pin S15D)
        \param msbPins Applicable bits: pins S17A..S25C (LSB bit 0 - pin S17A, bit 1 - pin S17B,..., MSB bit 18 - pin S25D)
        \param lsbValues Values for first group: pins S1A...S15D (0 or 1)
        \param msbValues Values for second group: pins S17A...S25D (0 or 1)
    */
    void setValues(int lsbPins, int msbPins, int lsbValues, int msbValues);

    /// Get value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin value (0 or 1)
    */
    unsigned int getValue(const char *pin);

    /// Get values for all pins simultaneously (see bit arithmetic)
    /*!
        \param lsbValues Values for first group: pins S1A...S15D (0 for input, 1 for output)
        \param msbValues Values for second group: pins S17A...S25D (0 for input, 1 for output)
    */
    void getValues(int &lsbValues, int &msbValues);

    /// Get directions for all pins simultaneously
    /*!
        \return GpioReturn structure
    */
    GpioReturn getValues();

    /// Get CPU pin number
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return CPU pin number
    */
    unsigned int getPinNumber(const char* pin);
};

#endif
