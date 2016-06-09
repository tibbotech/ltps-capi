#include "drivers/ci2c.h"

#include "global.h"

int Ci2c::open_x( uint16_t _busn) {
 int f;
 char fpath[ PATH_MAX];
 sprintf( fpath, SYS_I2C_PFX"%d", _busn);
 if ( ( f = open( fpath, O_RDWR)) < 0) return( -errno);
 return( f);  }

int Ci2c::set_bus( uint16_t _busn) {
 int f;
 if ( this->busn == _busn && this->f_rw >= 0) return( 1);
 if ( this->f_rw >= 0) close( this->f_rw);
 this->busn = this->f_rw = -1;
 if ( ( f = this->open_x( _busn)) < 0) return( f);
 // FIX: optionally set I2C PEC
 // FIX: optionally set 10BIT_ADDR
 this->f_rw = f;
 this->busn = _busn;
 return( 1);  }

// map { name, id }
int Ci2c::set_bus( const char *_buss, i2cmap_t *_map) {
 int f, cnt, ret = -1/*, i*/;
 int busn;
 DIR *dp;
 struct dirent *ep;
 char tmps0[ NAME_MAX], tmps1[ NAME_MAX];
 i2cmap_t *c_mapp, *mapps = _map;
 if ( _buss == NULL) return( -1);
 for ( int cnt = 0; ( c_mapp = ( mapps + cnt)) && c_mapp->name; cnt++) {
   if ( strcasecmp( c_mapp->name, _buss) != 0) continue;
   return( this->set_bus( c_mapp->id));
 }
 if ( ( dp = opendir( SCS_I2C_PFX)) == NULL) return( -1);
 while ( ( ep = readdir( dp))) {
   if ( strcmp( ep->d_name, ".") == 0) continue;
   if ( strcmp( ep->d_name, "..") == 0) continue;
   memset( tmps0, 0, NAME_MAX);
   sprintf( tmps0, "%s%s/name", SCS_I2C_PFX, ep->d_name);
   if ( ( f = open( tmps0, O_RDONLY)) < 0) continue;
   memset( tmps0, 0, NAME_MAX);
   cnt = read( f, tmps1, NAME_MAX - 1);
   close( f);
   if ( cnt < 4) continue;
   if ( tmps1[ cnt - 1] == 0x0A) tmps1[ --cnt] = '\0';
   if ( strcasecmp( tmps1 + sizeof( char)*3, _buss) != 0) continue;
   busn = atoi( ep->d_name + sizeof( char)*4);
   if ( busn < 0) continue;
   ret = this->set_bus( busn);
   break;  }
 closedir( dp);
 return( ret);  }

int Ci2c::set_slave( uint16_t _addr) {
 long addr = _addr;
 if ( this->f_rw < 0) return( 0);
 if ( this->addr == ( long)_addr) return( 1);
 this->addr = -1;
 if ( ( ioctl( this->f_rw, I2C_SLAVE_FORCE, addr)) < 0) return( -errno);
 this->addr = _addr;
 return( 1);  }
