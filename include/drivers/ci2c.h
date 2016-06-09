// This I Love
// (C) dvorkin@tibbo.com, 2015

#ifndef __CI2C_H__
#define __CI2C_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_DIR_R I2C_SMBUS_READ
#define I2C_DIR_W I2C_SMBUS_WRITE

struct i2cmap_t {
 const char *name;
 uint8_t id;
};

class Ci2c {

private:

 long addr;

protected:

 int f_rw;
 int busn;
 int open_x( uint16_t _busn);

public:

 Ci2c( void) {  this->busn = this->addr = this->f_rw = -1;  }
 Ci2c( Ci2c &_o) {  this->busn = _o.busn;  this->addr = this->f_rw = -1;  }
 ~Ci2c( void) {
   if ( this->f_rw >= 0) close( this->f_rw);
 }
 
 // ret: 1 - OK, <0 - errno
 int set_bus( uint16_t _busn);
 int set_bus( const char *_buss, i2cmap_t *_map = NULL);
 int cur_bus( void) {  return( this->busn);  }

 // NOTE: it may take too long time to read/write i2c device
 // especially if there are alot of legs/registers
 // so make this functions non-blockable

 // ret: > 0 number of bytes R/W, 0 - not opened (?), < 0 - errno
 virtual int R1b( uint16_t _addr, uint8_t _reg, uint8_t  &_b) = 0;
 virtual int W1b( uint16_t _addr, uint8_t _reg, uint8_t   _b) = 0;
 virtual int R2b( uint16_t _addr, uint8_t _reg, uint16_t &_b) = 0;
 virtual int W2b( uint16_t _addr, uint8_t _reg, uint16_t  _b) = 0;
 // max block size == 32bytes
 virtual int Rbb( uint16_t _addr, uint8_t _reg, uint8_t *_b, uint8_t _blen) = 0;
 virtual int Wbb( uint16_t _addr, uint8_t _reg, uint8_t *_b, uint8_t _blen) = 0;
 // quick type R/W (command-reg is data also)
 virtual int Rqb( uint16_t _addr, uint8_t _reg, uint8_t *_b, uint8_t _blen) = 0;
 virtual int Wqb( uint16_t _addr, uint8_t _reg, uint8_t *_b, uint8_t _blen) = 0;

 // ret: 1 - OK, 0 - not opened, < 0 - errno
 // don't really need to call explicitely.
 // call only for test if device exists or not
 // BUT currently it does not guaranteed it will give an error "no such device"
 // due to SMBUS i2c driver realization. R* or W* functions may give it later
 // so it's better to probe device with R1b( _addr, 0x00, b)
 // _addr may be 7 or 10 bits
 int set_slave( uint16_t _addr);

};

#endif
