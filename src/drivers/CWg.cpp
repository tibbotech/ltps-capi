#include "CWg.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SYS_TWG_PFX                "/sys/twg-"

int CWg::x_open( const char *_sock, const char *_what) {
 int f;
 char path[ PATH_MAX];
 sprintf( path, SYS_TWG_PFX"%s/%s", _sock, _what);
 if ( ( f = open( path, O_RDONLY/*|O_DIRECT*/)) < 0) return( -1);
 return( f);  }

int CWg::init( const char *_sock) {
 int f;
 if ( !_sock) return( -1);
 if ( strcmp( this->sock, _sock) != 0) this->x_close();
 if ( this->f_rw >= 0) return( 0);
 if ( ( f = this->x_open( _sock, "data")) < 0) return( f);
 this->f_rw = f;
 this->null_sock();
 strcpy( this->sock, _sock);
 return( 0);  }

int CWg::R( wg_data_t &_rbuf) {
 int ret = 0;
 if ( !this->f_rw) this->f_rw = this->x_open( this->sock, "data");
 if ( !this->f_rw) return( -1);
 ret = read( this->f_rw, &_rbuf, sizeof( wg_data_t));
 return( ( ret > 0 ? 1 : ret));  }

int CWg::mode_get( void) {
 static char ss[ PATH_MAX];
 int f = this->x_open( this->sock, "mode");
 if ( !f) return( -1);
 memset( ss, 0, PATH_MAX);
 int ret = read( f, ss, PATH_MAX);
 close( f);
 if ( ret < 0) return( -1);
 return( atoi( ss));  }

int CWg::mode_set( uint8_t _mode) {
 static char ss[ PATH_MAX];
 int f = this->x_open( this->sock, "mode");
 if ( !f) return( -1);
 memset( ss, 0, PATH_MAX);
 sprintf( ss, "%d", _mode);
 int ret = write( f, ss, PATH_MAX);
 close( f);
 if ( ret < 0) return( -1);
 return( 0);  }

int CWg::out0_get( void) {
 static char ss[ PATH_MAX];
 int f = this->x_open( this->sock, "out0");
 if ( !f) return( -1);
 memset( ss, 0, PATH_MAX);
 int ret = read( f, ss, PATH_MAX);
 close( f);
 if ( ret < 0) return( -1);
 return( atoi( ss));  }

int CWg::out0_set( uint8_t _val) {
 static char ss[ PATH_MAX];
 int f = this->x_open( this->sock, "out0");
 if ( !f) return( -1);
 memset( ss, 0, PATH_MAX);
 sprintf( ss, "%d", _val);
 int ret = write( f, ss, PATH_MAX);
 close( f);
 if ( ret < 0) return( -1);
 return( 0);  }

