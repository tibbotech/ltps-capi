/*!
    \example tibbit29.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/temperature.h"

int main()
{
    Temperature temp;
    TempData data;

    temp.getTemperature("s1", data);

    if (data.status == EXIT_SUCCESS)
        printf("Temperature = %f degrees Celsius\n", data.temp);
    else
        printf("%s\n", data.error);

    return 0;
}
