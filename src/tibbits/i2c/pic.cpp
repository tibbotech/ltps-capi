/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "drivers/cpin.h"
#include "drivers/ci2c_smbus.h"
#include "tibbits/i2c/pic.h"

#include "global.h"
#include "lutils.h"

#include <string>
#include <algorithm>

uint8_t readPic(int bus, uint16_t addr)
{
    Ci2c_smbus i2c;

    int res = i2c.set_bus(bus);

    if (res != 1)
    {
        printf("PIC set I2C bus errno: %i\n", res);
        return 0;
    }

    uint8_t data[2];
    uint8_t ret = 0;

    memset(&data, 0, 2);

    data[0] = (addr >> 8) & 0xFF;
    data[1] = addr & 0xFF;

    res = i2c.Wbb(PIC16F1824::I2C_ADDRESS, PIC16F1824::CMD_R, data, 2);

    res += i2c.R1b(PIC16F1824::I2C_ADDRESS, 0x00, ret);

    if (ret != 3)
    {
        printf("Error while reading data for PIC\n");
        return 0;
    }

    return ret;
}

void writePic(int bus, uint16_t addr, uint8_t data)
{
    Ci2c_smbus i2c;

    int res = i2c.set_bus(bus);

    if (res != 1)
    {
        printf("PIC set I2C bus errno: %i\n", res);
        return;
    }

    uint8_t str[3];
    memset(&str, 0, 3);

    str[0] = (addr >> 8) & 0xFF;
    str[1] = addr & 0xFF;
    str[2] = data;

    res = i2c.Wbb(PIC16F1824::I2C_ADDRESS, PIC16F1824::CMD_W, str, 3);

    if (res != 3)
        printf("Error while writing data for PIC\n");
}

Pic::Pic()
{

}

Pic::~Pic()
{

}

void Pic::initPic(const char *socket, PicFreq freq)
{
    std::string sock(socket);
    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    int busn = Lutils::getI2CBusNum(socket);
    int gpin_c = Lutils::readInteger(PINS_FILE, "CPU", std::string(sock + "C").c_str());

    if (busn == -1)
    {
        printf("I2C bus for socket %s not found\n", socket);
        return;
    }

    if (gpin_c == 0)
    {
        printf("GPIO pins for socket %s not found\n", socket);
        return;
    }

    initPic(busn, gpin_c, freq);
}

void Pic::initPic(int busn, int gpin_c, PicFreq freq)
{
    CPin gpio;
    if (gpio.init(gpin_c))
    {
        printf("PIC GPIO initialization error\n");
        return;
    }

    if (gpio.dir_get() == PIN_DIR_I)
        gpio.dir_set(PIN_DIR_O);

    gpio.W(0);
    usleep(5000);
    gpio.W(1);

    usleep(25000);

    writePic(busn, PIC16F1824::APFCON0, 0x28);
    writePic(busn, PIC16F1824::APFCON1, 0x00);
    writePic(busn, PIC16F1824::CCPTMRS0, 0x24);

    switch (freq)
    {
    case _32MHz:
        writePic(busn, PIC16F1824::OSCCON, 0xF0); //< 32 MHz
        break;
    case _16MHz:
        writePic(busn, PIC16F1824::OSCCON, 0x7A); //< 16 MHz
        break;
    case _8MHz:
        writePic(busn, PIC16F1824::OSCCON, 0x72); //< 8 MHz
        break;
    default:
        writePic(busn, PIC16F1824::OSCCON, 0xF0); //< 32 MHz
        break;
    }

    writePic(busn, PIC16F1824::ADCON1, 0xF0);
}

void Pic::configurePwm(const char *socket, int channel)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        configurePwm(busn, channel);
}

void Pic::configurePwm(int busn, int channel)
{
    if (channel == 1)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISC);
        writePic(busn, PIC16F1824::TRISC, val & 0xDF);

        val = readPic(busn, PIC16F1824::ANSELC);
        writePic(busn, PIC16F1824::ANSELC, val & 0xDF);

        val = readPic(busn, PIC16F1824::TRISA);
        writePic(busn, PIC16F1824::TRISA, val | 0x10);

        val = readPic(busn, PIC16F1824::ANSELA);
        writePic(busn, PIC16F1824::ANSELA, val & 0xEF);
    }

    if (channel == 2)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISC);
        writePic(busn, PIC16F1824::TRISC, (val & 0xF7) | 0x10);

        val = readPic(busn, PIC16F1824::ANSELC);
        writePic(busn, PIC16F1824::ANSELC, val & 0xF7);
    }

    if (channel == 3)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISA);
        writePic(busn, PIC16F1824::TRISA, val & 0xFB);

        val = readPic(busn, PIC16F1824::ANSELA);
        writePic(busn, PIC16F1824::ANSELA, val & 0xFB);
    }
}

void Pic::configureAdc(const char *socket, int channel)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        configureAdc(busn, channel);
}

void Pic::configureAdc(int busn, int channel)
{
    if (channel == 1)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISA);
        writePic(busn, PIC16F1824::TRISA, val | 0x10);

        val = readPic(busn, PIC16F1824::ANSELA);
        writePic(busn, PIC16F1824::ANSELA, val | 0x10);

        val = readPic(busn, PIC16F1824::TRISC);
        writePic(busn, PIC16F1824::TRISC, val | 0x20);
    }

    if (channel == 2)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISC);
        writePic(busn, PIC16F1824::TRISC, val | 0x18);

        val = readPic(busn, PIC16F1824::ANSELC);
        writePic(busn, PIC16F1824::ANSELC, val | 0x08);
    }

    if (channel == 3)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISA);
        writePic(busn, PIC16F1824::TRISA, val | 0x04);

        val = readPic(busn, PIC16F1824::ANSELA);
        writePic(busn, PIC16F1824::ANSELA, val | 0x04);
    }

    if (channel == 4)
    {
        uint8_t val = readPic(busn, PIC16F1824::TRISC);
        writePic(busn, PIC16F1824::TRISC, val | 0x04);

        val = readPic(busn, PIC16F1824::ANSELC);
        writePic(busn, PIC16F1824::ANSELC, val | 0x04);
    }

    uint8_t valAd = readPic(busn, PIC16F1824::ADCON1);

    valAd |= 0x03;
    valAd &= 0xFB;

    writePic(busn, PIC16F1824::ADCON1, valAd);

    uint8_t valFvr = readPic(busn, PIC16F1824::FVRCON);

    valFvr &= 0xFC;
    valFvr |= 0x83;

    writePic(busn, PIC16F1824::FVRCON, valFvr);
}

void Pic::startPwm(const char *socket, int channel, int pulse, int period, int prescaler)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        startPwm(busn, channel, pulse, period, prescaler);
}

void Pic::startPwm(int busn, int channel, int pulse, int period, int prescaler)
{
    stopPwm(busn, channel);

    uint8_t value, rCCPxN, rCCPRxL, rTxCON, rPRx;

    if (pulse > 1023)
        pulse = 1023;

    if (period > 1023)
        period = 1023;

    rCCPxN = 0x0C;

    pulse -= 1;
    period /= 4;
    period -= 1;

    rPRx = period;
    rCCPRxL = pulse / 4;

    // DCxBx value
    rCCPxN = rCCPxN | ((pulse & 0x0003) * 16);

    // Set prescaler and enable the timer
    value = 0;
    switch (prescaler)
    {
    case 64:
        value |= 3;
        break;
    case 16:
        value |= 2;
        break;
    case 4:
        value |= 1;
        break;
    default:
        break;
    }

    rTxCON = value;

    if (channel == 1)
    {
        writePic(busn, PIC16F1824::PR2, rPRx); // Load the PRx register with the PWM period value
        writePic(busn, PIC16F1824::CCPR1L, rCCPRxL); // Load the CCPRxL register and the DCxBx bits of the CCPxCON register, with the PWM duty cycle value
        writePic(busn, PIC16F1824::CCP1CON, rCCPxN); // Load DCxBx value and configure CCP1
        writePic(busn, PIC16F1824::T2CON, rTxCON | 0x04); // Set Timer
    }

    if (channel == 2)
    {
        writePic(busn, PIC16F1824::PR4, rPRx); // Load the PRx register with the PWM period value
        writePic(busn, PIC16F1824::CCPR2L, rCCPRxL); // Load the CCPRxL register and the DCxBx bits of the CCPxCON register, with the PWM duty cycle value
        writePic(busn, PIC16F1824::CCP2CON, rCCPxN); // Load DCxBx value and configure CCP1
        writePic(busn, PIC16F1824::T4CON, rTxCON | 0x04); // Set Timer
    }

    if (channel == 3)
    {
        writePic(busn, PIC16F1824::PR6, rPRx); // Load the PRx register with the PWM period value
        writePic(busn, PIC16F1824::CCPR3L, rCCPRxL); // Load the CCPRxL register and the DCxBx bits of the CCPxCON register, with the PWM duty cycle value
        writePic(busn, PIC16F1824::CCP3CON, rCCPxN); // Load DCxBx value and configure CCP1
        writePic(busn, PIC16F1824::T6CON, rTxCON | 0x04); // Set Timer
    }
}

void Pic::stopPwm(const char *socket, int channel)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
        printf("I2C bus for socket %s not found\n", socket);
    else
        stopPwm(busn, channel);
}

void Pic::stopPwm(int busn, int channel)
{
    if (channel == 1)
    {
        uint8_t val = readPic(busn, PIC16F1824::LATC);
        writePic(busn, PIC16F1824::LATC, val & 0xDF);

        writePic(busn, PIC16F1824::CCP1CON, 0x00);

        val = readPic(busn, PIC16F1824::T2CON);
        writePic(busn, PIC16F1824::T2CON, val & 0xFB);
    }

    if (channel == 2)
    {
        uint8_t val = readPic(busn, PIC16F1824::LATC);
        writePic(busn, PIC16F1824::LATC, val & 0xF7);

        writePic(busn, PIC16F1824::CCP2CON, 0x00);

        val = readPic(busn, PIC16F1824::T4CON);
        writePic(busn, PIC16F1824::T4CON, val & 0xFB);
    }

    if (channel == 3)
    {
        uint8_t val = readPic(busn, PIC16F1824::LATC);
        writePic(busn, PIC16F1824::LATC, val & 0xFB);

        writePic(busn, PIC16F1824::CCP3CON, 0x00);

        val = readPic(busn, PIC16F1824::T6CON);
        writePic(busn, PIC16F1824::T6CON, val & 0xFB);
    }
}

int Pic::getAdcVoltage(const char *socket, int channel)
{
    int busn = Lutils::getI2CBusNum(socket);

    if (busn == -1)
    {
        printf("I2C bus for socket %s not found\n", socket);
        return 0;
    }
    else
        return getAdcVoltage(busn, channel);
}

int Pic::getAdcVoltage(int busn, int channel)
{
    uint8_t value, adcl, adch;

    if (channel == 0)
        value = 0x0C;

    if (channel == 1)
        value = 0x1D;

    if (channel == 2)
        value = 0x09;

    if (channel == 3)
        value = 0x19;

    writePic(busn, PIC16F1824::ADCON0, value);

    /// Start conversion
    writePic(busn, PIC16F1824::ADCON0, value | 0x03);
    uint8_t val = 0;
    while (true)
    {
        val = readPic(busn, PIC16F1824::ADCON0);

        if ((val & 0x02) != 0x1)
            break; //< Conversion done

        usleep(5000);
    }

    adch = readPic(busn, PIC16F1824::ADRESH);
    adcl = readPic(busn, PIC16F1824::ADRESL);

    return (adch * 256 + adcl) * 400000 / 100000;
}
