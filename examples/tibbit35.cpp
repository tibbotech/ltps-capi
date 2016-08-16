/*!
    \example tibbit35.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/pressure.h"

int main()
{
    Pressure pres;
    PresData data;

    pres.getData("s1", data);

    if (data.status == EXIT_SUCCESS)
    {
        printf("Pressure = %f mm Hg\n", data.pressure);
        printf("Temperature = %f degrees Celsius\n", data.temperature);
    }
    else
        printf("%s\n", data.error);

    return 0;
}
