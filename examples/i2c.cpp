/*!
    \example i2c.cpp
*/

#include <string.h> // memset
#include <stdio.h> // printf

#include "ltps/capi/ci2c_smbus.h"
#include "ltps/capi/lutils.h"

int main()
{
    Ci2c_smbus i2c;

    int res = i2c.set_bus("-s01");

    if (res != 1)
        return 1;

    uint8_t data[2];
    memset(&data, 0, 2);

    i2c.Rbb(0x27, 0x00, data, 2); // Read block of 2 bytes
    printf("Block #1: %X", data[0]);
    printf("Block #2: %X", data[1]);

    uint8_t block = 0xFF;
    i2c.W1b(0x27, 0x00, block); // Write block of 1 byte

    return 0;
}
