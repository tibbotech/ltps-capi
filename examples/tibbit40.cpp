/*!
    \example tibbit40.cpp
*/

#include <stdio.h> // printf

#include "ltps/potentiometer.h"

int main()
{
    Potentiometer potent;

    printf("Set impedance to 30 kOhm of 50 kOhm\n"); // Tibbit 40_3
    potent.setImpedance("s1", 30000, _50kOhm);
}
