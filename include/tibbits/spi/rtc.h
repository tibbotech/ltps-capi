/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LRTC_H__
#define __LRTC_H__

#include <stdint.h>

/*!
    \struct RtcTemp
    \brief Struct with temperature for DS3234
*/

struct RtcTemp
{
    /// Temperature in C degrees
    float temp;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};

/*!
    \struct TimeStamp
    \brief Struct with time for RTC Tibbit
*/

struct TimeStamp
{
    // Seconds (0...59)
    uint8_t sec;

    // Minutes (0...59)
    uint8_t min;

    // Hours (0...23)
    uint8_t hour;

    // Day of the month (1...31)
    uint8_t mday;

    // Month (1...12)
    uint8_t mon;

    // Year
    int year;

    // Day of the week (1...7)
    uint8_t wday;

    // Saylight saving time (0...1)
    uint8_t isdst;

    // Year in short notation (auto calculation)
    uint8_t year_s;
};

/*!
    \class Rtc
    \brief Class for RTC tibbit control (Tibbit #42)
*/

class Rtc
{
public:

    Rtc();

    virtual ~Rtc();

    /// Get current temperature in Celsius degrees
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param temp RtcTemp data struct
    */
    void getTemperature(const char *socket, RtcTemp &temp);
};

#endif
