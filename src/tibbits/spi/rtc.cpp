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

    void stopAlarm(const char *socket, bool alarm1, RtcResult &result)
    {
        memset(&result, 0, sizeof result);

        char* error;
        Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

        if (spi)
        {
            uint8_t reg_val = 0;

            int len = 2;

            uint8_t w[len];
            uint8_t r[len];

            memset(&w, 0, sizeof w);
            memset(&r, 0, sizeof r);

            w[0] = DS3234::STATUS_READ;

            int res = spi->WR(w, r, len);

            if (alarm1)
                reg_val = r[1] & ~DS3234::ALARM1_FLAG;
            else
                reg_val = r[1] & ~DS3234::ALARM2_FLAG;

            memset(&w, 0, sizeof w);
            memset(&r, 0, sizeof r);

            w[0] = DS3234::STATUS_WRITE;
            w[1] = reg_val;

            res += spi->WR(w, r, len);

            if (res != len * 2)
            {
                result.status = EXIT_FAILURE;

                if (alarm1)
                    result.error = "Checksum error while clear Alarm 1 triggered flag for RTC";
                else
                    result.error = "Checksum error while clear Alarm 2 triggered flag for RTC";
            }

            result.status = EXIT_SUCCESS;
        }
        else
        {
            result.status = EXIT_FAILURE;
            result.error = error;
        }
    }

    void isAlarmTriggered(const char *socket, bool alarm1, RtcAlarmsStatus &status)
    {
        char* error;
        Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

        if (spi)
        {
            int len = 2;

            uint8_t w[len];
            uint8_t r[len];

            memset(&w, 0, sizeof w);
            memset(&r, 0, sizeof r);

            w[0] = DS3234::STATUS_READ;

            int res = spi->WR(w, r, len);

            if (res != len)
            {
                status.status = EXIT_FAILURE;

                if (alarm1)
                    status.error = "Checksum error while get Alarm 1 triggered flag for RTC";
                else
                    status.error = "Checksum error while get Alarm 2 triggered flag for RTC";
            }

            if (alarm1)
                status.triggered = r[1] & DS3234::ALARM1_FLAG;
            else
                status.triggered = r[1] & DS3234::ALARM2_FLAG;

            status.status = EXIT_SUCCESS;
        }
        else
        {
            status.status = EXIT_FAILURE;
            status.error = error;
        }
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

        RtcTime t;
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

void Rtc::setAlarm1(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags)
{
    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 5;

        uint8_t t[len];
        memset(&t, 0, sizeof t);

        t[0] = DS3234::A1_WRITE;
        t[1] = RtcPrivate::decToBcd(alarm.sec) | flags.seconds << 7;
        t[2] = RtcPrivate::decToBcd(alarm.min) | flags.minites << 7;
        t[3] = RtcPrivate::decToBcd(alarm.hour) | flags.hours << 7;
        t[4] = RtcPrivate::decToBcd(alarm.day) | flags.days << 7 | flags.wdays << 6;

        int res = spi->WR(t, t, len);

        if (res != len)
        {
            flags.status = EXIT_FAILURE;
            flags.error = "Checksum error while set Alarm 1 time for RTC";
            return;
        }

        flags.status = EXIT_SUCCESS;
        flags.error = NULL;
    }
    else
    {
        flags.status = EXIT_FAILURE;
        flags.error = error;
    }
}

void Rtc::setAlarm2(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags)
{
    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 4;

        uint8_t t[len];
        memset(&t, 0, sizeof t);

        t[0] = DS3234::A2_WRITE;
        t[1] = RtcPrivate::decToBcd(alarm.min) | flags.minites << 7;
        t[2] = RtcPrivate::decToBcd(alarm.hour) | flags.hours << 7;
        t[3] = RtcPrivate::decToBcd(alarm.day) | flags.days << 7 | flags.wdays << 6;

        int res = spi->WR(t, t, len);

        if (res != len)
        {
            flags.status = EXIT_FAILURE;
            flags.error = "Checksum error while set Alarm 2 time for RTC";
            return;
        }

        flags.status = EXIT_SUCCESS;
        flags.error = NULL;
    }
    else
    {
        flags.status = EXIT_FAILURE;
        flags.error = error;
    }
}

void Rtc::getAlarm1(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags)
{
    memset(&flags, 0, sizeof flags);

    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 5;

        uint8_t n[len];
        uint8_t w[len];
        uint8_t t[len -1]; // second, minute, hour, (w)day

        memset(&n, 0, sizeof n);
        memset(&w, 0, sizeof w);
        memset(&t, 0, sizeof t);

        w[0] = DS3234::A1_READ;

        int res = spi->WR(w, n, len);

        if (res != len)
        {
            flags.status = EXIT_FAILURE;
            flags.error = "Checksum error while get Alarm 1 time for RTC";
            return;
        }

        flags.seconds = (n[1] & 0x80) >> 7;
        t[0] = RtcPrivate::bcdToDec(n[1] & 0x7F);

        flags.minites = (n[2] & 0x80) >> 7;
        t[1] = RtcPrivate::bcdToDec(n[2] & 0x7F);

        flags.hours = (n[3] & 0x80) >> 7;
        t[2] = RtcPrivate::bcdToDec(n[3] & 0x7F);

        flags.days = (n[4] & 0x80) >> 7;
        t[3] = RtcPrivate::bcdToDec(n[4] & 0x3F);

        flags.wdays = (n[4] & 0x40) >> 6;

        alarm.sec = t[0];
        alarm.min = t[1];
        alarm.hour = t[2];
        alarm.day = t[3];

        flags.status = EXIT_SUCCESS;
    }
    else
    {
        flags.status = EXIT_FAILURE;
        flags.error = error;
    }
}

void Rtc::getAlarm2(const char *socket, RtcAlarm &alarm, RtcAlarmFlags& flags)
{
    memset(&flags, 0, sizeof flags);

    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 4;

        uint8_t n[len];
        uint8_t w[len];
        uint8_t t[len -1]; // second, minute, hour, (w)day

        memset(&n, 0, sizeof n);
        memset(&w, 0, sizeof w);
        memset(&t, 0, sizeof t);

        w[0] = DS3234::A2_READ;

        int res = spi->WR(w, n, len);

        if (res != len)
        {
            flags.status = EXIT_FAILURE;
            flags.error = "Checksum error while get Alarm 2 time for RTC";
            return;
        }

        flags.minites = (n[1] & 0x80) >> 7;
        t[0] = RtcPrivate::bcdToDec(n[1] & 0x7F);

        flags.hours = (n[2] & 0x80) >> 7;
        t[1] = RtcPrivate::bcdToDec(n[2] & 0x7F);

        flags.days = (n[3] & 0x80) >> 7;
        t[2] = RtcPrivate::bcdToDec(n[3] & 0x3F);

        flags.wdays = (n[3] & 0x40) >> 6;

        alarm.sec = 0;
        alarm.min = t[0];
        alarm.hour = t[1];
        alarm.day = t[2];

        flags.status = EXIT_SUCCESS;
    }
    else
    {
        flags.status = EXIT_FAILURE;
        flags.error = error;
    }
}

void Rtc::enableAlarms(const char* socket, bool alarm1, bool alarm2, RtcResult &result)
{
    memset(&result, 0, sizeof result);

    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 2;

        uint8_t w[len];
        uint8_t r[len];

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::CONTROL_READ;

        int res = spi->WR(w, r, len);

        uint8_t reg_val = r[1];

        if (!alarm1 && !alarm2)
            reg_val = r[1] | 0x4;
        if (alarm1 && alarm2)
            reg_val = r[1] | 0x7;
        if (alarm1 && !alarm2)
            reg_val = r[1] | 0x5;
        if (!alarm1 && alarm2)
            reg_val = r[1] | 0x6;

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::CONTROL_WRITE;
        w[1] = reg_val;

        res += spi->WR(w, r, len);

        if (res != len * 2)
        {
            result.status = EXIT_FAILURE;
            result.error = "Checksum error while activate alarms for RTC";
            return;
        }

        result.status = EXIT_SUCCESS;
    }
    else
    {
        result.status = EXIT_FAILURE;
        result.error = error;
    }
}

void Rtc::stopAlarm1(const char *socket, RtcResult &result)
{
    RtcPrivate::stopAlarm(socket, true, result);
}

void Rtc::stopAlarm2(const char *socket, RtcResult &result)
{
    RtcPrivate::stopAlarm(socket, false, result);
}

void Rtc::isAlarm1Triggered(const char *socket, RtcAlarmsStatus &status)
{
    RtcPrivate::isAlarmTriggered(socket, true, status);
}

void Rtc::isAlarm2Triggered(const char *socket, RtcAlarmsStatus &status)
{
    RtcPrivate::isAlarmTriggered(socket, false, status);
}

void Rtc::setSramValue(const char *socket, RtcSram &sram)
{
    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 2;

        uint8_t w[len];
        uint8_t r[len];

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::SRAM_ADDRESS;
        w[1] = sram.address;

        int res = spi->WR(w, r, len);

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::SRAM_DATA_WRITE;
        w[1] = sram.value;

        res += spi->WR(w, r, len);

        if (res != len * 2)
        {
            sram.status = EXIT_FAILURE;
            sram.error = "Checksum error while write value to SRAM for RTC";
            return;
        }

        sram.status = EXIT_SUCCESS;
        sram.error = NULL;
    }
    else
    {
        sram.status = EXIT_FAILURE;
        sram.error = error;
    }
}

void Rtc::getSramValue(const char *socket, RtcSram &sram)
{
    char* error;
    Cspi *spi = Lutils::getInstance().getSpiPointer(socket, &error);

    if (spi)
    {
        int len = 2;

        uint8_t w[len];
        uint8_t r[len];

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::SRAM_ADDRESS;
        w[1] = sram.address;

        int res = spi->WR(w, r, len);

        memset(&w, 0, sizeof w);
        memset(&r, 0, sizeof r);

        w[0] = DS3234::SRAM_DATA_READ;

        res += spi->WR(w, r, len);

        if (res != len * 2)
        {
            sram.status = EXIT_FAILURE;
            sram.error = "Checksum error while read value from SRAM for RTC";
            return;
        }

        sram.value = r[1];
        sram.status = EXIT_SUCCESS;
        sram.error = NULL;
    }
    else
    {
        sram.status = EXIT_FAILURE;
        sram.error = error;
    }
}
