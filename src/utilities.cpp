/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "utilities.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define MAX_BUF_SIZE    255

std::string Lutilites::readString(std::string file, std::string section, std::string param)
{
    std::ifstream fl;
    fl.open(file.c_str());

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

            if (section.empty())
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
                return val;
            }
        }
    }

    if (fl.is_open())
        fl.close();

    return "";
}

int Lutilites::readInteger(std::string file, std::string section, std::string param)
{
    std::string value = Lutilites::readString(file, section, param);

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

std::string Lutilites::getI2CName(std::string bus)
{
    std::string busName = "-s";

    std::string sockNum = bus.substr(bus.find("s") + 1, bus.length() - bus.find("s") + 1);

    if (atoi(sockNum.c_str()) < 10)
        busName.append("0");

    busName.append(sockNum);

    return busName;
}
