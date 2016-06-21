/*!
    \example tibbit30.cpp
*/

#include "ltps/humidity.h"

int main()
{
    Humidity hum;
    HumData data;

    hum.getData("s1", data);

    printf("RH = %f percents\n", data.humidity);
    printf("Temperature = %f degrees Celsius\n", data.temperature);
    printf("Status = %i\n", data.status);
}
