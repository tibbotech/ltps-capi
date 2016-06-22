/*!
    \example tibbit29.cpp
*/

#include <stdio.h> // printf

#include "ltps/temperature.h"

int main()
{
    Temperature temp;

    printf("Temperature = %f degrees Celsius\n", temp.getTemperature("s1"));
}
