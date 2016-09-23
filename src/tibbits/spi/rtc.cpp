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

        w[0] = DS3234::TEMP_REG;

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

void Rtc::setTime(const char *socket, RtcClock &time)
{
    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        uint8_t century;
        int len = 8;

        uint8_t TimeDate[len];
        memset(&TimeDate, 0, sizeof TimeDate);

        if (time.time.year > 2000)
        {
            century = 0x80;
            time.time.year_s = time.time.year - 2000;
        }
        else
        {
            century = 0;
            time.time.year_s = time.time.year - 1900;
        }

        TimeDate[0] = DS3234::CLOCK_WRITE;
        TimeDate[1] = RtcPrivate::decToBcd(time.time.sec);
        TimeDate[2] = RtcPrivate::decToBcd(time.time.min);
        TimeDate[3] = RtcPrivate::decToBcd(time.time.hour);
        TimeDate[4] = RtcPrivate::decToBcd(time.time.wday);
        TimeDate[5] = RtcPrivate::decToBcd(time.time.mday);
        TimeDate[6] = RtcPrivate::decToBcd(time.time.mon) + century;
        TimeDate[7] = RtcPrivate::decToBcd(time.time.year_s);

        int res = spi->WR(TimeDate, TimeDate, len);

        if (res != len)
        {
            time.status = EXIT_FAILURE;
            time.error = "Checksum error while set time for RTC";
            return;
        }

        time.status = EXIT_SUCCESS;
        time.error = NULL;
    }
    else
    {
        time.status = EXIT_FAILURE;
        time.error = error;
    }
}

void Rtc::getTime(const char *socket, RtcClock &time)
{
    memset(&time, 0, sizeof time);

    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        uint8_t century = 0;
        uint16_t year_full = 2000;

        int len = 8;

        TimeStamp t;
        uint8_t TimeDate[len];
        uint8_t w[len];

        memset(&t, 0, sizeof t);
        memset(&TimeDate, 0, sizeof TimeDate);
        memset(&w, 0, sizeof w);

        w[0] = DS3234::CLOCK_READ;

        int res = spi->WR(w, TimeDate, len);

        if (res != len)
        {
            time.status = EXIT_FAILURE;
            time.error = "Checksum error while get time for RTC";
            return;
        }

        t.sec = RtcPrivate::bcdToDec(TimeDate[1]);
        t.min = RtcPrivate::bcdToDec(TimeDate[2]);
        t.hour = RtcPrivate::bcdToDec(TimeDate[3]);
        t.mday = RtcPrivate::bcdToDec(TimeDate[5]);

        uint8_t n = TimeDate[6];
        t.mon = RtcPrivate::bcdToDec(n & 0x1F);
        century = (n & 0x80) >> 7;

        if (century == 1)
            year_full = 2000 + RtcPrivate::bcdToDec(TimeDate[7]);
        else
            year_full = 1900 + RtcPrivate::bcdToDec(TimeDate[7]);

        t.year = year_full;

        t.wday = RtcPrivate::bcdToDec(TimeDate[4]);
        t.year_s = RtcPrivate::bcdToDec(TimeDate[7]);

        time.time = t;
        time.status = EXIT_SUCCESS;
    }
    else
    {
        time.status = EXIT_FAILURE;
        time.error = error;
    }
}
