/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LPIC_H__
#define __LPIC_H__

/*!
    \enum PicFreq
    \brief Possible frequencies of internal oscillator for PWM
*/

enum PicFreq
{
    _8MHz = 8,
    _16MHz = 16,
    _32MHz = 32
};

/*!
    \class PIC
    \brief Class for PWM/ADC PIC16F1824 tibbit control (Tibbit #16, Tibbit #17 and Tibbit #31)
*/

class Pic
{
public:

    Pic();

    virtual ~Pic();

    /// PIC initialization (call before PIC first usage)
    /*!
        \param bus I2C bus number
        \param gpin GPIO C-pin bus number
        \param freq Frequency of internal oscillator for PWM
    */
    void initPic(int bus, int gpin_c, PicFreq freq);

    /// Configure PIC as PWM (supported by Tibbit's #16, #17 and #31)
    /*!
        \param bus I2C bus number
        \param channel 1, 2 or 3
    */
    void configurePwm(int bus, int channel);

    /// Configure PIC as ADC (supported by Tibbit #31)
    /*!
        \param bus I2C bus number
        \param channel 1, 2, 3 or 4
    */

    void configureAdc(int bus, int channel);

    /// Start PWM (configurePwm must called before)
    /*!
        \param bus I2C bus number
        \param channel Channel number (1, 2 or 3)
        \param pusle PWM pulse width is defined as the base frequency clock count (0, 1, 2...1023)
        \param period PWM period is defined as the base frequency clock count (0, 4, 8...1023)
        \param prescaler Prescaler affects the base frequency (the period is multiplied to the prescaler) (1, 4, 16 or 64)
    */
    void startPwm(int bus, int channel, int pulse, int period, int prescaler);

    /// Stop PWM (initPic or configurePwm must called before)
    /*!
        \param bus I2C bus number
        \param channel Channel number (1, 2 or 3)
    */
    void stopPwm(int bus, int channel);

    /// Get ADC voltage (0...4000 mV) (configureAdc must called before)
    /*!
        \param bus I2C bus number
        \param channel Channel number (1, 2, 3 or 4)
        \return Voltage in mV
    */
    int getAdcVoltage(int bus, int channel);

private:

    uint8_t readPic(int bus, uint16_t addr);

    void writePic(int bus, uint16_t addr, uint8_t data);
};

#endif
