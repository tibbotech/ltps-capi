/*!
    \example tibbit28.cpp
*/

#include <stdio.h> // printf
#include <stdlib.h> // exit statuses

#include "ltps/capi/light.h"

int main()
{
    Light light;
    LightData data;

    light.getIllumination("s1", data);
    if (data.status == EXIT_SUCCESS)
        printf("Illumination = %i lx\n", data.ill);
    else
        printf("%s\n", data.error);
}
