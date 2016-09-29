/*!
    \copyright Tibbo Technology Inc
    \author Vitaly Gribko (vitaliy.gribko@tibbo.com)
*/

#ifndef __LRTC_H__
#define __LRTC_H__

#include <stdint.h>


/*!
    \struct RtcTime
    \brief Struct with time for clock in RTC Tibbit
*/

struct RtcTime
{
    // Seconds (0...59)
    uint8_t sec;

    // Minutes (0...59)
    uint8_t min;

    // Hours (0...23)
    uint8_t hour;

    // Day of the week (1...7)
    uint8_t wday;

    // Day of the month (1...31)
    uint8_t mday;

    // Month (1...12)
    uint8_t mon;

    // Year
    int year;

    // Year in short notation (auto calculation)
    uint8_t year_s;

    // Saylight saving time (0...1)
    uint8_t isdst;
};


/*!
    \struct RtcAlarm
    \brief Struct with time for alarms in RTC Tibbit
*/

struct RtcAlarm
{
    // Seconds (0...59)
    uint8_t sec;

    // Minutes (0...59)
    uint8_t min;

    // Hours (0...23)
    uint8_t hour;

    // Day of the week (1...7) or day of the month (1...31)
    uint8_t day;
};


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
    \struct RtcClock
    \brief Struct with time for DS3234
*/

struct RtcClock
{
    /// Time
    RtcTime time;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};


/*!
    \struct RtcAlarmFlags
    \brief Struct with alarm flags for RTC
*/

struct RtcAlarmFlags
{
    /// Seconds (false to enable, true to disable) - does not matter for Alarm 2
    bool seconds;

    /// Minutes (false to enable, true to disable)
    bool minites;

    /// Hours (false to enable, true to disable)
    bool hours;

    /// Days (false to enable, true to disable)
    bool days;

    /// Day of week == true/day of month == false
    bool wdays;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};


/*!
    \struct RtcResult
    \brief Struct with some operations result for RTC
*/

struct RtcResult
{
    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};


/*!
    \struct RtcAlarmsStatus
    \brief Struct with alarms triggered status for RTC
*/

struct RtcAlarmsStatus
{
    /// Return true, if alarm is triggered
    bool triggered;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
};


/*!
    \struct RtcSram
    \brief Struct with SRAM registers for RTC
*/

struct RtcSram
{
    /// SRAM address (256 bytes: 0x00...0xFF)
    uint8_t address;

    /// Value
    uint8_t value;

    /// Return status (EXIT_SUCCESS or EXIT_FAILURE)
    int status;

    /// String error if something goes wrong (NULL for success)
    const char* error;
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

    /// Set current time
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param time RtcClock data struct
    */
    void setTime(const char* socket, RtcClock &time);

    /// Get current time
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param time RtcClock data struct
    */
    void getTime(const char* socket, RtcClock &time);

    /// Set time for Alarm 1
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param alarm Alarm time in RtcAlarm struct
        \param flags Component to be checked to trigger the alarm
    */
    void setAlarm1(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags);

    /// Set time for Alarm 2
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param alarm Alarm time in RtcAlarm struct
        \param flags Component to be checked to trigger the alarm
    */
    void setAlarm2(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags);

    /// Get time for Alarm 1
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param alarm Alarm time in RtcAlarm struct
        \param flags RtcResult struct result
    */
    void getAlarm1(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags);

    /// Get time for Alarm 2
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param alarm Alarm time in RtcAlarm struct
        \param flags RtcResult struct result
    */
    void getAlarm2(const char* socket, RtcAlarm& alarm, RtcAlarmFlags& flags);

    /// Activate alarms pin (if slot activated, INT/MISO pin will be set to LOW when alarms triggered)
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param alarm1 Alarm 1 activate slot
        \param alarm2 Alarm 2 activate slot
        \param result Function execution result
    */
    void enableAlarms(const char* socket, bool alarm1, bool alarm2, RtcResult& result);

    /// Clear Alarm 1 triggered status
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param result Function execution result
    */
    void stopAlarm1(const char* socket, RtcResult& result);

    /// Clear Alarm 2 triggered status
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param result Function execution result
    */
    void stopAlarm2(const char* socket, RtcResult& result);

    /// Check Alarm 1 triggered status
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param status RtcAlarmsStatus data struct
    */
    void isAlarm1Triggered(const char* socket, RtcAlarmsStatus& status);

    /// Check Alarm 2 triggered status
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param status RtcAlarmsStatus data struct
    */
    void isAlarm2Triggered(const char* socket, RtcAlarmsStatus& status);

    /// Write value to SRAM memory
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param sram RtcSram data struct
    */
    void setSramValue(const char* socket, RtcSram& sram);

    /// Get value from SRAM memory
    /*!
        \param socket SPI bus name (eg: s1, s15)
        \param sram RtcSram data struct
    */
    void getSramValue(const char* socket, RtcSram& sram);
};

#endif
