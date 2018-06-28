/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/i2c/adc-16bits.h"

#include "global.h"
#include "lutils.h"

#include <stdio.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

namespace Adc16Private
{
    std::vector<std::string> inited;
    std::map<std::string, float*> coeffs;

    void waitIntPin(CPin *gpio)
    {
        while (gpio->R() == 0)
        {
            usleep(1000);
        }
    }

    void readCoeffs(std::string socket, Ci2c_smbus *i2c, CPin *gpio)
    {
        uint8_t value[16];
        memset(&value, 0, sizeof value);

        i2c->W1b(ADS1100::I2C_ADDRESS, ADS1100::COMP, 0x00);

        waitIntPin(gpio);

        i2c->Rbb(ADS1100::I2C_ADDRESS, ADS1100::COMP, value, sizeof value);

        float errorCompensation[4];
        for (int i = 0; i < 4; i++)
        {
            uint8_t step = i * 4;

            errorCompensation[i] = ((value[1 + step] * 0x10000) + (value[2 + step] * 0x100) + value[3 + step]) / (double) 1000000;
            if (value[0 + step] & 0x80)
            {
                errorCompensation[i] = -1 * (errorCompensation[i] + (value[0 + step] & 0x7F) * 0x1000000);
            }
            else
            {
                errorCompensation[i] = errorCompensation[i] + value[0 + step] * 0x1000000;
            }
        }

        coeffs[socket] = errorCompensation;

        waitIntPin(gpio);
    }

    int initTibbit(std::string socket, Ci2c_smbus *i2c, char **error)
    {
        CPin *gpio_c = Lutils::getInstance().getGpioPointer((socket + "C").c_str(), error);

        if (gpio_c)
        {
            gpio_c->dir_set(PIN_DIR_O);

            gpio_c->W(0);
            usleep(1000); //< 1 ms
            gpio_c->W(1);

            usleep(20000); //< 20 ms
        }
        else
        {
            return -1;
        }

        CPin *gpio_d = Lutils::getInstance().getGpioPointer((socket + "D").c_str(), error);

        if (gpio_d)
        {
            gpio_d->dir_set(PIN_DIR_I);
        }
        else
        {
            return -1;
        }

        i2c->W1b(ADS1100::I2C_ADDRESS, ADS1100::SRAM, 0x00);

        uint8_t data[16];
        memset(&data, 0, sizeof data);

        int res = i2c->Rbb(ADS1100::I2C_ADDRESS, ADS1100::SRAM, data, sizeof data);

        if ((res != sizeof data) || (strcmp((const char*) data, ADS1100::TBTFW) == 0))
        {
            asprintf(error, "FW mismatched");
            return -1;
        }

        uint8_t conf[2];
        conf[1] = 0x1C; //< Single mode + 16 bits +  Gain = 1

        res = 0;

        for (int i = 0; i < 4; i++)
        {
            conf[0] = i;
            res += i2c->Wbb(ADS1100::I2C_ADDRESS, ADS1100::CONF, conf, sizeof conf);
            waitIntPin(gpio_d);
        }

        if (res != 8)
        {
            asprintf(error, "Set configuration error");
            return -1;
        }

        readCoeffs(socket, i2c, gpio_d);

        return 0;
    }

    CPin* getIntPin(std::string socket)
    {
        std::string sock(socket);

        char *error;
        CPin *gpio_d = Lutils::getInstance().getGpioPointer((sock + "D").c_str(), &error);

        if (gpio_d)
        {
            if (gpio_d->dir_get() == PIN_DIR_O)
                gpio_d->dir_set(PIN_DIR_I);

            return gpio_d;
        }
        else
        {
            return NULL;
        }
    }
}

Adc16::Adc16()
{

}

Adc16::~Adc16()
{

}

void Adc16::getVoltage(const char* socket, unsigned int channel, Adc16Data &adc)
{
    std::string sock(socket);
    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    channel -= 1;

    memset(&adc, 0, sizeof adc);

    char* error;
    Ci2c_smbus *i2c = Lutils::getInstance().getI2CPointer(socket, &error);

    if (i2c)
    {
        if (std::find(Adc16Private::inited.begin(), Adc16Private::inited.end(), sock) == Adc16Private::inited.end())
        {
            if (Adc16Private::initTibbit(sock, i2c, &error) == -1)
            {
                adc.status = EXIT_FAILURE;
                adc.error = error;
                return;
            }

            Adc16Private::inited.push_back(sock);
            usleep(150000); // 150 ms
        }

        if (std::find(Adc16Private::inited.begin(), Adc16Private::inited.end(), sock) != Adc16Private::inited.end())
        {
            float tmp = 0;
            const float LSB_mV = 0.00030517578125;

            uint8_t data[3];
            memset(&data, 0, sizeof data);

            bool busy_wait = false, f = false;

            while (!busy_wait)
            {
                int res = i2c->W1b(ADS1100::I2C_ADDRESS, ADS1100::OUT + 0x01, channel);
                if (res != 1)
                {
                    adc.status = EXIT_FAILURE;
                    adc.error = "I2C set out register error";
                    return;
                }

                Adc16Private::waitIntPin(Adc16Private::getIntPin(sock));

                res = i2c->Rbb(ADS1100::I2C_ADDRESS, ADS1100::OUT, data, sizeof data);

                if (res != 3)
                {
                    adc.status = EXIT_FAILURE;
                    adc.error = "I2C get out register error";
                    return;
                }

                if (data[2] & 0x80)
                {
                    busy_wait = true;
                    continue;
                }

                if (f)
                {
                    tmp = data[0] * 256 + data[1];
                    break;
                }
                else
                {
                    uint8_t conf[2];
                    conf[0] = channel;
                    conf[1] = 0x9C; //< Single mode + 16 bits +  Gain = 1

                    res = i2c->Wbb(ADS1100::I2C_ADDRESS, ADS1100::CONF, conf, sizeof conf);
                    if (res != sizeof conf)
                    {
                        adc.status = EXIT_FAILURE;
                        adc.error = "Configuration error";
                        return;
                    }

                    Adc16Private::waitIntPin(Adc16Private::getIntPin(sock));
                    usleep(150000); // 150 ms

                    f = true;
                    continue;
                }

            };

            if (tmp <= 0x7FFF)
            {
                f = false;
            }
            else
            {
                f = true;
                tmp = 0xFFFF - tmp + 1;
            }

            float volts = Adc16Private::coeffs.at(sock)[channel] * tmp;
            volts = volts/(float) 1000000;
            volts = volts + tmp*LSB_mV;

            if (f)
            {
                volts *= -1;
            }

            adc.voltage = volts;
            adc.status = EXIT_SUCCESS;

            return;
        }

        adc.status = EXIT_FAILURE;
        adc.error = "Tibbit initialisation error";
    }
    else
    {
        adc.status = EXIT_FAILURE;
        adc.error = error;
    }
}
