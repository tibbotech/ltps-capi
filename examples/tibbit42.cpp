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

    TimeStamp ts;
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
}
