/*!
    \example tibbit30.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/humidity.h"

int main()
{
    Humidity hum;
    HumData data;

    hum.getData("s1", data);

    if (data.status == EXIT_SUCCESS)
    {
        printf("RH = %f percents\n", data.humidity);
        printf("Temperature = %f degrees Celsius\n", data.temperature);
    }
    else
        printf("%s\n", data.error);

    return 0;
}
