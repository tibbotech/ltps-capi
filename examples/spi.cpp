/*!
    \example spi.cpp
*/

#include <string.h> // memset
#include <stdio.h> // printf

#include "ltps/capi/Cspi.h"
#include "ltps/capi/lutils.h"

#define MAX_BUF 100

int main( int argc, char *argv[]) {
 Cspi x;
 x.set_dev( "-s05", 0);
 x.set_mode( SPI_MODE_0);
 x.set_bits( 8);
 x.settings_set();
 x.settings_get();
 printf( "Bits\t:%d\n", x.cur_bits());
 printf( "Speed\t:%d\n", x.cur_speed());
 printf( "Delay\t:%d\n", x.cur_delay());
 printf( "Mode\t:%02x\n", x.cur_mode());
 int ret, len;
 uint8_t w[ MAX_BUF], r[ MAX_BUF];

 memset( w, 0, MAX_BUF);
 memset( r, 0, MAX_BUF);
 // read 8 bytes starting from 0x11 address
 w[ 0] = 0x11;
 len = 8;

 ret = x.WR( w, r, len);
 printf( "ret:%d\n", ret);
 if ( ret < 0) printf( "(%s)\n", strerror( -ret));
 // result is in "r"
 return( 0);  }
