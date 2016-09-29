/*!
    \example tibbit42.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses
#include <unistd.h> // sleep

#include "ltps/capi/rtc.h"

int main()
{
    Rtc rtc;


    for (int i = 0; i < 10; i++)
    {
        RtcTemp data;
        rtc.getTemperature("s1", data); // Get current temperature

        if (data.status == EXIT_SUCCESS)
            printf("Temperature = %f degrees Celsius\n", data.temp);
        else
        {
            printf("%s\n", data.error);
            break;
        }

        sleep(1);
    }


    RtcTime ts;
    memset(&ts, 0, sizeof ts);

    ts.sec = 0;
    ts.min = 20;
    ts.hour = 15;
    ts.mday = 23;
    ts.mon = 9;
    ts.year = 2016;
    ts.wday = 5;

    RtcClock clock;
    memset(&clock, 0, sizeof clock);

    clock.time = ts;
    rtc.setTime("s1", clock); // Set current time

    if (clock.status == EXIT_SUCCESS)
        printf("Time has been successfully installed\n");
    else
        printf("%s\n", clock.error);

    for (int i = 0; i < 10; i++)
    {
        memset(&clock, 0, sizeof clock);
        rtc.getTime("s1", clock); // Get current time

        if (clock.status == EXIT_SUCCESS)
            printf("The time now is: %02d sec, %02d min, %02d hour, %d wday, %02d mday, %02d mon, %d year, %02d syear\n",
                   clock.time.sec, clock.time.min, clock.time.hour, clock.time.wday,
                   clock.time.mday, clock.time.mon, clock.time.year, clock.time.year_s);
        else
        {
            printf("%s\n", clock.error);
            break;
        }

        sleep(1);
    }


    RtcResult result;
    memset(&result, 0, sizeof result);

    rtc.stopAlarm1("s1", result); // Clear Alarm 1 triggered status

    if (result.status == EXIT_SUCCESS)
        printf("Alarm 1 triggered status successfully cleared\n");
    else
        printf("%s\n", result.error);


    memset(&result, 0, sizeof result);

    rtc.stopAlarm2("s1", result); // Clear Alarm 2 triggered status

    if (result.status == EXIT_SUCCESS)
        printf("Alarm 2 triggered status successfully cleared\n");
    else
        printf("%s\n", result.error);


    RtcAlarm alarm1;

    alarm1.sec = 30;
    alarm1.min = 21;
    alarm1.hour = 15;
    alarm1.day = 23;

    RtcAlarmFlags flags;
    memset(&flags, 0, sizeof flags);

    /*flags.seconds = false;
    flags.minites = false;
    flags.hours = false;
    flags.days = false;
    flags.wdays = false;*/

    rtc.setAlarm1("s1", alarm1, flags); // Set Alarm 1 time

    if (flags.status == EXIT_SUCCESS)
        printf("Alarm 1 time successfully set\n");
    else
        printf("%s\n", flags.error);

    memset(&alarm1, 0, sizeof alarm1);
    memset(&flags, 0, sizeof flags);

    rtc.getAlarm1("s1", alarm1, flags); // Get Alarm 1 time

    if (flags.status == EXIT_SUCCESS)
        printf("Alarm 1 time: %02d sec, %02d min, %02d hour, %02d wday, flag seconds = %d, flag minutes = %d, flag hours = %d, flag days = %d, flag wdays = %d\n",
            alarm1.sec, alarm1.min, alarm1.hour, alarm1.day, flags.seconds, flags.minites, flags.hours, flags.days, flags.wdays);
    else
        printf("%s\n", flags.error);


    RtcAlarm alarm2;

    alarm2.sec = 0; // Does not matter for Alarm 2
    alarm2.min = 21;
    alarm2.hour = 15;
    alarm2.day = 23;

    memset(&flags, 0, sizeof flags);

    /*flags.minites = false;
    flags.hours = false;
    flags.days = false;
    flags.wdays = false;*/

    rtc.setAlarm2("s1", alarm2, flags); // Set Alarm 2 time

    if (flags.status == EXIT_SUCCESS)
        printf("Alarm 2 time successfully set\n");
    else
        printf("%s\n", flags.error);

    memset(&alarm2, 0, sizeof alarm2);
    memset(&flags, 0, sizeof flags);

    rtc.getAlarm2("s1", alarm2, flags); // Get Alarm 2 time

    if (flags.status == EXIT_SUCCESS)
        printf("Alarm 2 time: %02d min, %02d hour, %02d wday, flag minutes = %d, flag hours = %d, flag days = %d, flag wdays = %d\n",
               alarm2.min, alarm2.hour, alarm2.day, flags.minites, flags.hours, flags.days, flags.wdays);
    else
        printf("%s\n", flags.error);


    memset(&result, 0, sizeof result);

    rtc.enableAlarms("s1", true, true, result); // Activate alarms pin (INT/MISO pin will be set to LOW when alarms triggered)

    if (result.status == EXIT_SUCCESS)
        printf("Alarms pin was successfully activated\n");
    else
        printf("%s\n", result.error);


    while (true)
    {
        memset(&clock, 0, sizeof clock);
        rtc.getTime("s1", clock); // Get current time

        if (clock.status == EXIT_SUCCESS)
            printf("The time now is: %02d sec, %02d min, %02d hour, %d wday, %02d mday, %02d mon, %d year, %02d syear\n",
                   clock.time.sec, clock.time.min, clock.time.hour, clock.time.wday,
                   clock.time.mday, clock.time.mon, clock.time.year, clock.time.year_s);
        else
        {
            printf("%s\n", clock.error);
            break;
        }

        bool alarm1 = false, alarm2 = false;

        RtcAlarmsStatus status;
        memset(&status, 0, sizeof status);

        rtc.isAlarm1Triggered("s1", status); // Check Alarm 1 triggered status

        if (status.status == EXIT_SUCCESS)
            alarm1 = status.triggered;
        else
        {
            printf("%s\n", status.error);
            break;
        }

        printf("Alarm 1 status: %d\n", alarm1);

        memset(&status, 0, sizeof status);
        rtc.isAlarm2Triggered("s1", status); // Check Alarm 2 triggered status

        if (status.status == EXIT_SUCCESS)
            alarm2 = status.triggered;
        else
        {
            printf("%s\n", status.error);
            break;
        }

        printf("Alarm 2 status: %d\n", alarm2);

        if (alarm1 && alarm2)
        {
            printf("All alarms triggered!\n");
            break;
        }
        else
            sleep(10);
    }


    RtcSram sram;
    memset(&sram, 0, sizeof sram);

    sram.address = 0x16;
    sram.value = 0x79;

    rtc.setSramValue("s1", sram); // Write value to SRAM

    if (sram.status == EXIT_SUCCESS)
        printf("Value successfully write to SRAM register\n");
    else
        printf("%s\n", sram.error);


    memset(&sram, 0, sizeof sram);

    sram.address = 0x16;

    rtc.getSramValue("s1", sram); // Read value from SRAM

    if (sram.status == EXIT_SUCCESS)
        printf("Value in SRAM register: %X\n", sram.value);
    else
        printf("%s\n", sram.error);
}
