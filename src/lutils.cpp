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
#include <fstream>
#include <algorithm>

#include "lutils.h"
#include "global.h"

#define MAX_BUF_SIZE        255

int Lutils::getI2CBusNum(const char* socket)
{
    std::string sock(socket);

    std::transform(sock.begin(), sock.end(), sock.begin(), ::toupper);

    if (Lutils::readString(PINS_FILE, "I2C", sock.c_str()) && strlen(Lutils::readString(PINS_FILE, "I2C", sock.c_str())) > 0)
        return Lutils::readInteger(PINS_FILE, "I2C", sock.c_str()); //< HW I2C

    std::transform(sock.begin(), sock.end(), sock.begin(), ::tolower);

    std::string busName = "-s";

    std::string sockNum = sock.substr(sock.find("s") + 1, sock.length() - sock.find("s") + 1);

    if (atoi(sockNum.c_str()) < 10)
        busName.append("0");

    busName.append(sockNum);

    int file, cnt;
    int busn;
    DIR *dp;
    struct dirent *ep;
    char tmps0[NAME_MAX], tmps1[NAME_MAX];

    if ((dp = opendir(SCS_I2C_PFX)) == NULL)
        return -1;

    while ((ep = readdir(dp)))
    {
        if (strcmp(ep->d_name, ".") == 0)
            continue;

        if (strcmp(ep->d_name, "..") == 0)
            continue;

        memset(tmps0, 0, NAME_MAX);
        sprintf(tmps0, "%s%s/name", SCS_I2C_PFX, ep->d_name);

        if ((file = open(tmps0, O_RDONLY)) < 0)
            continue;

        memset(tmps0, 0, NAME_MAX);
        cnt = read(file, tmps1, NAME_MAX - 1);

        close(file);

        if (cnt < 4)
            continue;

        if (tmps1[cnt - 1] == 0x0A)
            tmps1[--cnt] = '\0';

        if (strcasecmp(tmps1 + sizeof(char) * 3, busName.c_str()) != 0)
            continue;

        busn = atoi(ep->d_name + sizeof(char) * 4);

        if (busn < 0)
            continue;

        return busn;
        break;
    }

    closedir( dp);

    return -1;
}

const char *Lutils::readString(const char* file, const char* section, const char* param)
{
    std::ifstream fl;
    fl.open(file);

    if (fl.is_open())
    {
        char buf[MAX_BUF_SIZE];
        bool sect = false;

        while(fl.getline(buf, sizeof buf))
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

                fl.close();
                return val.c_str();
            }
        }
    }

    if (fl.is_open())
        fl.close();

    return "";
}

int Lutils::readInteger(const char *file, const char* section, const char* param)
{
    std::string value = Lutils::readString(file, section, param);

    try
    {
        if (!value.empty())
            return atoi(value.c_str());
        else
            return 0;
    }
    catch(...) {}

    return 0;
}
