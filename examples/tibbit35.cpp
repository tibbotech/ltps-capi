/*!
    \example tibbit35.cpp
*/

#include <stdio.h> // printf

#include "ltps/pressure.h"

int main()
{
    Pressure pres;
    PresData data;

    pres.getData("s1", data);

    printf("Pressure = %f mm Hg\n", data.pressure);
    printf("Temperature = %f degrees Celsius\n", data.temperature);
}
