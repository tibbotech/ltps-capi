/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#include "tibbits/spi/rtc.h"

#include "global.h"
#include "lutils.h"

namespace RtcPrivate
{
    uint8_t decToBcd(const uint8_t val)
    {
        return ((val / 10 * 16) + (val % 10));
    }

    uint8_t bcdToDec(const uint8_t val)
    {
        return ((val / 16 * 10) + (val % 16));
    }
}

Rtc::Rtc()
{

}

Rtc::~Rtc()
{

}

void Rtc::getTemperature(const char *socket, RtcTemp &temp)
{
    memset(&temp, 0, sizeof temp);

    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int8_t nint;
        int len = 3;

        uint8_t w[len], r[len];

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = 0x11;

        int res = spi->WR(w, r, len);

        if (res != len)
        {
            temp.status = EXIT_FAILURE;
            temp.error = "Checksum error while get temperature for RTC";
            return;
        }

        uint8_t temp_msb = r[1];
        uint8_t temp_lsb = r[2] >> 6;

        if ((temp_msb & 0x80) != 0)
            nint = temp_msb | ~((1 << 8) - 1);
        else
            nint = temp_msb;

        temp.temp = 0.25 * temp_lsb + nint;
        temp.status = EXIT_SUCCESS;
    }
    else
    {
        temp.status = EXIT_FAILURE;
        temp.error = error;
    }
}
