/*!
    \example tibbit13.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/adc-4-20.h"

int main()
{
    Adc420 adc;
    Adc420Data data;

    adc.getCurrent("s1", data);

    if (data.status == EXIT_SUCCESS)
        printf("Current = %f A\n", (double) data.current);
    else
        printf("%s", data.error);

    return 0;
}
