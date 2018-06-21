/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LGLOBAL_H__
#define __LGLOBAL_H__

#define SYS_GPIO_PFX                "/sys/class/gpio/"

#define SYS_I2C_PFX                 "/dev/i2c-"
#define SCS_I2C_PFX                 "/sys/class/i2c-dev/"

/*!
    \namespace LTC2309
    \brief ADC I2C addresses (Tibbit #13)
*/

namespace LTC2309
{
    static const unsigned int I2C_ADDRESS = 0x08; // AD0 and AD1 setted to Low

    static const unsigned int UNIPOLAR_MODE = 0x08;

    static const unsigned int CH0 = 0x80;
    static const unsigned int CH2 = 0x90;
    static const unsigned int CH4 = 0xA0;
    static const unsigned int CH6 = 0xB0;
}

/*!
    \namespace MCP4728
    \brief DAC I2C addresses (Tibbit #14)
*/

namespace MCP4728
{
    static const unsigned int I2C_ADDRESS = 0x60;

    static const unsigned int MULTI_WRITE = 0x40;
    static const unsigned int VREF_GX = 0x90;
}

/*!
    \namespace PIC16F1824
    \brief PIC16F1824 I2C addresses (Tibbit #16, #17, #31)
*/

namespace PIC16F1824
{
    static const unsigned int I2C_ADDRESS = 0x03;

    static const unsigned int CMD_R = 0x01;
    static const unsigned int CMD_W = 0x02;

    static const unsigned int APFCON0 = 0x011D;
    static const unsigned int APFCON1 = 0x011E;
    static const unsigned int CCPTMRS0 = 0x029E;

    static const unsigned int OSCCON = 0x0099;

    static const unsigned int TRISA = 0x008C;
    static const unsigned int TRISC = 0x008E;
    static const unsigned int ANSELA = 0x018C;
    static const unsigned int ANSELC = 0x018E;

    static const unsigned int LATC = 0x010E;
    static const unsigned int CCP1CON = 0x0293;
    static const unsigned int CCP2CON = 0x029A;
    static const unsigned int CCP3CON = 0x0313;
    static const unsigned int T2CON = 0x001C;
    static const unsigned int T4CON = 0x0417;
    static const unsigned int T6CON = 0x041E;

    static const unsigned int PR2 = 0x001B;
    static const unsigned int PR4 = 0x0416;
    static const unsigned int PR6 = 0x041D;
    static const unsigned int CCPR1L = 0x0291;
    static const unsigned int CCPR2L = 0x0298;
    static const unsigned int CCPR3L = 0x0311;

    static const unsigned int ADCON0 = 0x009D;
    static const unsigned int ADCON1 = 0x009E;
    static const unsigned int FVRCON = 0x0117;
    static const unsigned int ADRESL = 0x009B;
    static const unsigned int ADRESH = 0x009C;
}

/*!
    \namespace BH1721FVC
    \brief Ambient light sensor I2C addresses (Tibbit #28)
*/

namespace BH1721FVC
{
    static const unsigned int I2C_ADDRESS = 0x23;

    static const unsigned int POWEROFF = 0x00;
    static const unsigned int POWERON = 0x01;
    static const unsigned int HIGHRES = 0x12;
}

/*!
    \namespace MCP9808
    \brief Ambient temperature sensor I2C addresses (Tibbit #29)
*/

namespace MCP9808
{
    static const unsigned int I2C_ADDRESS = 0x18;

    static const unsigned int RESOLUTION = 0x08;
    static const unsigned int RESOLUSIONMODE = 0x01;
    static const unsigned int TA = 0x05;
}

/*!
    \namespace HIH6130
    \brief Ambient humidity/temperature meter I2C addresses (Tibbit #30)
*/

namespace HIH6130
{
    static const unsigned int I2C_ADDRESS = 0x27;
}

/*!
    \namespace MPL115A2
    \brief Ambient pressure/temperature meter I2C addresses (Tibbit #35)
*/

namespace MPL115A2
{
    static const unsigned int I2C_ADDRESS = 0x60;

    static const unsigned int REG_A0_MSB = 0x04;
    static const unsigned int REG_B1_MSB = 0x06;
    static const unsigned int REG_B2_MSB = 0x08;
    static const unsigned int REG_C12_MSB = 0x0A;

    static const unsigned int CMD_CONVERSION = 0x12;

    static const unsigned int PRESSURE_MSB = 0x00;
    static const unsigned int TEMPERATURE_MSB = 0x02;
}

/*!
    \namespace ADXL312
    \brief 3-axis accelerometer I2C addresses (Tibbit #36)
*/

namespace ADXL312
{
    static const unsigned int I2C_ADDRESS = 0x53;

    static const unsigned int REG_DEVID = 0x00;

    static const unsigned int POWER_CTL = 0x2D;
    static const unsigned int STANDBY_MODE = 0x00;
    static const unsigned int MEASUREMENT_MODE = 0x08;

    static const unsigned int DATA_FORMAT = 0x31;
    static const unsigned int FULL_RES_12G = 0x0B; //< 0x08 | 0x0B

    static const unsigned int FIFO_CTL = 0x38;
    static const unsigned int BYPASS_MODE = 0x80;

    static const unsigned int DATA_X0 = 0x32;
    static const unsigned int DATA_X1 = 0x33;

    static const unsigned int DATA_Y0 = 0x34;
    static const unsigned int DATA_Y1 = 0x35;

    static const unsigned int DATA_Z0 = 0x36;
    static const unsigned int DATA_Z1 = 0x37;

    static const unsigned int INT_ENABLE = 0x2E;
    static const unsigned int INT_FLAG = 0xFE;

    static const unsigned int SIGN_MASK = 0xE000;
    static const unsigned int DATA_MASK = 0x1FFF;
}

/*!
    \namespace MCP4561
    \brief Digital potentiometer I2C addresses (Tibbit #40)
*/

namespace MCP4561
{
    static const unsigned int I2C_ADDRESS = 0x2F;

    static const unsigned int VOLATILE_WIPER = 0x00;
    static const unsigned int NON_VOLATILE_WIPER = 0x20;

    static const unsigned int VOLATILE_TCON = 0x40;
}

/*!
    \namespace MCP23008
    \brief 8-bit port extender I2C addresses (Tibbit #41)
*/

namespace MCP23008
{
    static const unsigned int I2C_ADDRESS = 0x20;

    static const unsigned int IODIR = 0x00;
    static const unsigned int GPPU = 0x06;
    static const unsigned int GPIO = 0x09;
}

/*!
    \namespace DS3234
    \brief DS3234 RTC SPI addresses
*/

namespace DS3234
{
    static const unsigned int TEMP_REG = 0x11;

    static const unsigned int CLOCK_WRITE = 0x80;
    static const unsigned int CLOCK_READ = 0x00;

    static const unsigned int A1_WRITE = 0x87;
    static const unsigned int A1_READ = 0x07;

    static const unsigned int A2_WRITE = 0x8B;
    static const unsigned int A2_READ = 0x0B;

    static const unsigned int CONTROL_WRITE = 0x8E;
    static const unsigned int CONTROL_READ = 0x0E;

    static const unsigned int STATUS_WRITE = 0x8F;
    static const unsigned int STATUS_READ = 0x0F;

    static const unsigned int SRAM_ADDRESS = 0x98;

    static const unsigned int SRAM_DATA_WRITE = 0x99;
    static const unsigned int SRAM_DATA_READ = 0x19;

    static const unsigned int ALARM1_FLAG = 0x1;
    static const unsigned int ALARM2_FLAG = 0x2;
}

/*!
    \namespace ADS1100R
    \brief ADS1100 address for ADC 4...20 Tibbit
*/

namespace ADS1100R
{
    static const unsigned int I2C_ADDRESS = 0x48;
}

#endif
