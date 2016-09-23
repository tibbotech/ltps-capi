/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#include <string>
#include <iostream>
#include <algorithm>

#include "lutils.h"
#include "global.h"

#include "Ci2c.h"

#define MAX_BUF_SIZE            255
#define PINS_INI_FILE           "/opt/tps-shared/hwini/pins.ini"

Lutils::Lutils()
{
    m_fl.open(PINS_INI_FILE);
}

Lutils::~Lutils()
{
    if (m_fl.is_open())
        m_fl.close();

    for (std::map<const char*, int, CompareCStrings>::iterator it = m_si2c.begin(); it != m_si2c.end(); ++it)
        delete it->first;

    for (std::map<const char*, int, CompareCStrings>::iterator it = m_sspi.begin(); it != m_sspi.end(); ++it)
        delete it->first;

    for (std::map<const char*, int, CompareCStrings>::iterator it = m_sgpio.begin(); it != m_sgpio.end(); ++it)
        delete it->first;

    for (std::map<const char*, CPin*, CompareCStrings>::iterator it = m_pins.begin(); it != m_pins.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }

    for (std::map<const char*, Ci2c_smbus*, CompareCStrings>::iterator it = m_i2c.begin(); it != m_i2c.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }

    for (std::map<const char*, Cspi*, CompareCStrings>::iterator it = m_spi.begin(); it != m_spi.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }
}

Lutils &Lutils::getInstance()
{
    static Lutils lutils;

    return lutils;
}

int Lutils::getI2CBusNum(const char* socket)
{
    if (m_si2c.find(socket) != m_si2c.end())
        return m_si2c.at(socket);

    std::string sock(socket);

    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    if (Lutils::readString("I2C", sock.c_str()) && strlen(Lutils::readString("I2C", sock.c_str())) > 0)
    {
        char *data = new char(strlen(socket) + 1);
        strcpy(data, socket);

        int ret = Lutils::readInteger("I2C", sock.c_str()); //< HW I2C;
        m_si2c[data] = ret;
        return ret;
    }

    std::transform(sock.begin(), sock.end(), sock.begin(), ::tolower);

    std::string busName = "-s";

    std::string sockNum = sock.substr(sock.find("s") + 1, sock.length() - sock.find("s") + 1);

    if (atoi(sockNum.c_str()) < 10)
        busName.append("0");

    busName.append(sockNum);

    int res = Ci2c::find_bus(busName.c_str());

    if (res != -1)
    {
        char *data = new char(strlen(socket) + 1);
        strcpy(data, socket);

        m_si2c[data] = res;
    }

    return res;
}

int Lutils::getSpiBusNum(const char *socket)
{
    if (m_sspi.find(socket) != m_sspi.end())
        return m_sspi.at(socket);

    std::string sock(socket);

    std::transform(sock.begin(), sock.end(), sock.begin(), ::tolower);

    std::string busName = "-s";

    std::string sockNum = sock.substr(sock.find("s") + 1, sock.length() - sock.find("s") + 1);

    if (atoi(sockNum.c_str()) < 10)
        busName.append("0");

    busName.append(sockNum);

    int res = Cspi::find_bus(busName.c_str());

    if (res != -1)
    {
        char *data = new char(strlen(socket) + 1);
        strcpy(data, socket);

        m_sspi[data] = res;
    }

    return res;
}

int Lutils::readInteger(const char* section, const char* param)
{
    if (strcmp(section, "CPU") == 0)
    {
        if (m_sgpio.find(param) != m_sgpio.end())
            return m_sgpio.at(param);
    }

    std::string value = Lutils::readString(section, param);

    try
    {
        if (!value.empty())
        {
            char *data = new char(strlen(param) + 1);
            strcpy(data, param);

            int ret = atoi(value.c_str());
            m_sgpio[data] = ret;

            return ret;
        }
        else
            return 0;
    }
    catch(...) {}

    return 0;
}

CPin* Lutils::getGpioPointer(const char* pin, char **error)
{
    if (m_pins.find(pin) != m_pins.end())
        return m_pins.at(pin);
    else
    {
        CPin *cpin = new CPin();
        int res = cpin->init(readInteger("CPU", pin));
        if (!res)
        {
            char *data = new char(strlen(pin) + 1);
            strcpy(data, pin);

            m_pins[data] = cpin;

            return cpin;
        }
        else
        {
            delete cpin;
            asprintf(error, "GPIO %s PIN initialization error: %s", pin, strerror(abs(res)));
        }
    }

    return NULL;
}

Ci2c_smbus* Lutils::getI2CPointer(const char* socket, char **error)
{
    if (m_i2c.find(socket) != m_i2c.end())
        return m_i2c.at(socket);
    else
    {
        int busn = getI2CBusNum(socket);

        if (busn > -1)
        {
            Ci2c_smbus *i2c = new Ci2c_smbus();
            int res = i2c->set_bus(busn);

            if (res == 1)
            {
                char *data = new char(strlen(socket) + 1);
                strcpy(data, socket);

                m_i2c[data] = i2c;

                return i2c;
            }
            else
            {
                delete i2c;
                asprintf(error, "I2C set %s bus error: %s", socket, strerror(abs(res)));
                return NULL;
            }
        }
        else
            asprintf(error, "I2C bus for socket %s not found", socket);
    }

    return NULL;
}

Cspi* Lutils::getSpiPointer(const char* socket, char **error)
{
    if (m_spi.find(socket) != m_spi.end())
        return m_spi.at(socket);
    else
    {
        int busn = getSpiBusNum(socket);

        if (busn > -1)
        {
            Cspi *spi = new Cspi();
            int res = spi->set_dev(busn, 0);

            if (res == 1)
            {
                spi->settings_get();
                spi->set_mode(SPI_MODE_0);
                spi->set_bits(8);
                spi->settings_set();

                char *data = new char(strlen(socket) + 1);
                strcpy(data, socket);

                m_spi[data] = spi;

                return spi;
            }
            else
            {
                delete spi;
                asprintf(error, "SPI set %s bus error: %s", socket, strerror(abs(res)));
                return NULL;
            }
        }
        else
            asprintf(error, "SPI bus for socket %s not found", socket);
    }

    return NULL;
}

const char* Lutils::readString(const char* section, const char* param)
{
    if (m_fl.is_open())
    {
        char buf[MAX_BUF_SIZE];
        bool sect = false;

        m_fl.clear();
        m_fl.seekg(0);

        while(m_fl.getline(buf, sizeof buf))
        {
            std::string rdStr(buf);

            size_t com = rdStr.find(';');
            if (com != std::string::npos)
                rdStr = rdStr.substr(0, com);

            if ((rdStr.find('[') != std::string::npos) || (rdStr.find(']') != std::string::npos))
            {
                sect = false;
                rdStr.erase(std::remove(rdStr.begin(), rdStr.end(), ' '), rdStr.end());
            }

            if (section && strlen(section) > 0)
                sect = true;

            std::string par = "[" + std::string(section) + "]";
            if(rdStr.find(par) != std::string::npos)
                sect = true;

            if (rdStr.find("=") == std::string::npos)
                continue;

            std::string sep = rdStr.substr(0, rdStr.find("="));
            std::string remBack = sep;

            remBack.erase(std::remove(remBack.begin(), remBack.end(), '\t'), remBack.end());
            remBack.erase(std::remove(remBack.begin(), remBack.end(), '\n'), remBack.end());
            remBack.erase(std::remove(remBack.begin(), remBack.end(), ' '), remBack.end());

            if (sect && rdStr.find(sep) != std::string::npos && remBack == param)
            {
                std::string val = rdStr.substr(rdStr.find("=") + 1, rdStr.length() - rdStr.find("=") - 1);

                short shift = 0;
                for (unsigned short i = 0; i < val.length(); i++)
                {
                    if (val[i] == ' ')
                        shift = i + 1;
                    else
                        break;
                }

                val = val.substr(shift, val.length() - shift);

                val.erase(std::remove(val.begin(), val.end(), '\r'), val.end());
                val.erase(std::remove(val.begin(), val.end(), '\n'), val.end());
                val.erase(std::remove(val.begin(), val.end(), '\t'), val.end());

                return val.c_str();
            }
        }
    }

    return "";
}
