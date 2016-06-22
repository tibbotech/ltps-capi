/*!
    \example tibbit41.cpp
*/

#include <stdio.h> // printf

#include "ltps/portextender.h"

int main()
{
    Portextender pext;
    PortexData data;

    data.direction = false;
    data.pullup = false; //< It does not matter for output mode (see PortexData struct definition)

    for (int i = 1; i < 9; i++)
    {
        printf("Set pin %i to output with value = %i\n", i, i % 2);
        data.value = i % 2;
        pext.setData("s1", i, data);
    }
}
