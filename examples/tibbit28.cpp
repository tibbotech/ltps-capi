/*!
    \example tibbit28.cpp
*/

#include <stdio.h> // printf

#include "ltps/capi/light.h"

int main()
{
    Light lt;

    printf("Illumination = %i lx\n", lt.getIllumination("s1"));

    return 0;
}
