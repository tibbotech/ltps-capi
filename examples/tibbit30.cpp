/*!
    \example tibbit30.cpp
*/

#include <stdio.h> // printf

#include "ltps/capi/humidity.h"

int main()
{
    Humidity hum;
    HumData data;

    hum.getData("s1", data);

    printf("RH = %f percents\n", data.humidity);
    printf("Temperature = %f degrees Celsius\n", data.temperature);
    printf("Status = %i\n", data.status);

    return 0;
}
