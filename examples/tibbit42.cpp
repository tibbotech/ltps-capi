/*!
    \example tibbit41.cpp
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
        rtc.getTemperature("s1", data);

        if (data.status == EXIT_SUCCESS)
            printf("Temperature = %f degrees Celsius\n", data.temp);
        else
            printf("%s\n", data.error);

        sleep(1);
    }
}
