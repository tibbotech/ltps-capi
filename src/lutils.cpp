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
}

Lutils &Lutils::getInstance()
{
    static Lutils lutils;

    return lutils;
}

int Lutils::getI2CBusNum(const char* socket)
{
    if (m_i2c.find(socket) != m_i2c.end())
        return m_i2c.at(socket);

    std::string sock(socket);

    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    if (Lutils::readString("I2C", sock.c_str()) && strlen(Lutils::readString("I2C", sock.c_str())) > 0)
    {
        int ret = Lutils::readInteger("I2C", sock.c_str()); //< HW I2C;
        m_i2c[socket] = ret;
        return ret;
    }

    std::transform(sock.begin(), sock.end(), sock.begin(), ::tolower);

    std::string busName = "-s";

    std::string sockNum = sock.substr(sock.find("s") + 1, sock.length() - sock.find("s") + 1);

    if (atoi(sockNum.c_str()) < 10)
        busName.append("0");

    busName.append(sockNum);

    return Ci2c::find_bus(busName.c_str());
}

int Lutils::readInteger(const char* section, const char* param)
{
    if (strcmp(section, "CPU") == 0)
    {
        if (m_gpio.find(param) != m_gpio.end())
            return m_gpio.at(param);
    }

    std::string value = Lutils::readString(section, param);

    try
    {
        if (!value.empty())
        {
            int ret = atoi(value.c_str());
            m_gpio[param] = ret;

            return ret;
        }
        else
            return 0;
    }
    catch(...) {}

    return 0;
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

            std::string par = '[' + section + ']';
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
