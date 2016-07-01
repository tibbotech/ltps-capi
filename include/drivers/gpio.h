/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LGPIO_H__
#define __LGPIO_H__

/*!
    \enum GpioDir
    \brief Possible GPIO pin's directions
*/

enum GpioDir
{
    input,
    output
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
        \param direction Pin direction
        \return Result (0 on success)
    */
    int setDirection(const char *pin, GpioDir direction);

    /// Get direction
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin direction
    */
    GpioDir getDirection(const char *pin);

    /// Set value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \param value Pin value (0 or 1)
        \return Result (< 0 on error)
    */
    int setValue(const char* pin, unsigned int value);

    /// Get value
    /*!
        \param pin String name of pin (eg: S1A, S9D)
        \return Pin value (0 or 1)
    */
    unsigned int getValue(const char *pin);
};

#endif
