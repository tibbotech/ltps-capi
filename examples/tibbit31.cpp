/*!
    \example tibbit31.cpp
*/

#include <stdio.h> // printf
#include <unistd.h> // sleep

#include "ltps/pic.h"

int main()
{
    Pic pic;

    printf("Initialize PIC\n");
    pic.initPic("s1", _32MHz);

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

    sleep(1);

    for (int i = 1; i < 5; i++)
    {
        printf("Configure PIC as ADC at channel %i\n", i);
        pic.configureAdc("s1", i);
        sleep(1);

        printf("Channel %i = %i mV\n", i, pic.getAdcVoltage("s1", i));
    }
}
