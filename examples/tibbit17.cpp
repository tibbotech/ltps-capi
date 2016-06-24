/*!
    \example tibbit17.cpp
*/

#include <stdio.h> // printf
#include <unistd.h> // sleep

#include "ltps/capi/pic.h"

int main()
{
    Pic pic;

    printf("Initialize PIC\n");
    pic.initPic("s1", _16MHz);

    for (int i = 1; i < 4; i++)
    {
        printf("Configure PIC as PWM at channel %i\n", i);
        pic.configurePwm("s1", i);
        sleep(1);

        printf("Start PWM at channel %i (pusle = %i, period = %i, prescaler = %i)\n", i, 100, i * 300, 1);
        pic.startPwm("s1", i, 100, i * 300, 1);
        sleep(3);

        printf("Stop PWM at channel %i\n", i);
        pic.stopPwm("s1", i);
    }

    return 0;
}
