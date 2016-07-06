//
// 
// (C) dvorkin@tibbo.com, 2016

#ifndef __SPI_H__
#define __SPI_H__

#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <linux/spi/spidev.h>

class SPI {

private:

 long addr;
 uint32_t mode;
 uint8_t bits;
 uint32_t speed;
 uint16_t delay;
 struct spi_ioc_transfer tr;

protected:

 int f_rw;
 int busn;
 int x_open( uint16_t _busn, uint8_t _dev);
 int x_close( void) {
   if ( this->f_rw >= 0) close( this->f_rw);
   this->f_rw = -1;
 }

public:

 SPI( void) {
   this->busn = this->addr = this->f_rw = -1;
   this->bits = 8;  this->speed = 500000;  this->delay = 0;
 }
 SPI( SPI &_o) {  this->busn = _o.busn;  this->addr = this->f_rw = -1;  }
 ~SPI( void) {  this->x_close();  }
 
 int set_dev( uint16_t _busn, uint8_t _dev);
 // device# on TPS socket "sXX"
 int set_dev( const char *_sock, uint8_t _dev);

 int settings_get( void);
 int settings_set( void);

 // this methods are done for class properties access only
 // call settings_set()/settings_get() for sync with real device
 inline void set_bits( uint8_t _bits) {  this->bits = _bits;  }
 inline uint8_t cur_bits( void) {  return( this->bits);  }
 inline void set_speed( uint32_t _speed) {  this->speed = _speed;  }
 inline uint32_t cur_speed( void) {  return( this->speed);  }
 inline void set_delay( uint16_t _delay) {  this->delay = _delay;  }
 inline uint16_t cur_delay( void) {  return( this->delay);  }
 inline void set_mode( uint32_t _mode) {  this->mode = _mode;  }
 inline uint32_t cur_mode( void) {  return( this->mode);  }

 int cur_bus( void) {  return( this->busn);  }
 int cur_dev( void) {  return( this->addr);  }

 // I/O
 inline int R( uint8_t const *_b, size_t _l) {  this->WR( _b, NULL, _l);  }
 inline int W( uint8_t const *_b, size_t _l) {  this->WR( NULL, _b, _l);  }
 int WR( uint8_t const *_w, uint8_t const *_r, size_t _l);


 // -1 = not found
 static int find_bus( const char *_sock);

}; // class /

#endif

// modes
//SPI_CPHA				/* clock phase */
//SPI_CPOL				/* clock polarity */
//SPI_MODE_0	(0|0)			/* (original MicroWire) */
//SPI_MODE_1	(0|SPI_CPHA)
//SPI_MODE_2	(SPI_CPOL|0)
//SPI_MODE_3	(SPI_CPOL|SPI_CPHA)
//SPI_CS_HIGH				/* chipselect active high? */
//SPI_LSB_FIRST				/* per-word bits-on-wire */
//SPI_3WIRE				/* SI/SO signals shared */
//SPI_LOOP				/* loopback mode */
//SPI_NO_CS				/* 1 dev/bus, no chipselect */
//SPI_READY				/* slave pulls low to pause */
//SPI_TX_DUAL				/* transmit with 2 wires */
//SPI_TX_QUAD				/* transmit with 4 wires */
//SPI_RX_DUAL				/* receive with 2 wires */
