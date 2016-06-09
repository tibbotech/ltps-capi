// This I Love
// (C) dvorkin@tibbo.com, 2015

#ifndef __CPIN_H__
#define __CPIN_H__

#include <stdio.h>
#include <string.h>
#include <dirent.h>

class CPin {

protected:

 int pin;
 FILE *fp_rw;
 
 FILE *open_x( int _pin, const char *_what);
 
public:

 CPin( void) {  this->pin = 0;  this->fp_rw = NULL;  }
 ~CPin( void) {
   if ( this->fp_rw) fclose( this->fp_rw);
 }
 
 // call once
 // ret: 0 - OK, -1 - err
 int init( int _pin);
 int cur( void) {  return( this->pin);  }

 // ret: pin value = 0|1
 int R( void);
 // ret: fwrite return code | < 0 on error
 int W( int _val);

 // ret: -1 - err, 0 == PIN_DIR_I, 1 == PIN_DIR_O
 int dir_get( void);
 // ret: -1 - err, 0 - success
 int dir_set( int _dir);

};

#endif
