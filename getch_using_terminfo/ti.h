/*
 * =====================================================================================
 *
 *       Filename:  tinfo.h
 *
 *    Description: Term info header
 *
 *        Version:  1.0
 *        Created:  12/30/2020 06:35:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees Group
 *
 * =====================================================================================
 */

#ifndef __tinfo_h__
#define __tinfo_h__

#include <stdint.h>

#define MAGIC16     0432       /* Magic number for 16 bit ti file */
#define MAGIC32     01036      /* Magic number for 32 bit ti file */
#define HEADER_LEN  12         /* TI header length */
#define MAX_S32     0x7ffffff  /* Max signed 32 bit int */
#define MAX_S16     0x7fff     /* Max signed 16 bit int */

/* Return Codes For keys */
enum keyCodes { code_begain = 256 };

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getCh
 *  Description: get a char from user and not wait wait for Enter key to be pressed
 *  			 NON-CANONICAL MODE
 *  Returns: ascii decimal char value of key, or returns a code > 255 for keys
 *  F2 - F12, Home or arrows PgUp PgDn ..., or -1 if key is not found in terminfo db
 *	NOTE: getCh () should not be used with a char cause a char is 1 byte, and return
 *		  value of function keys will cause over flow, the correct way to use it is:
 *		  int ch = getCh ();
 * =====================================================================================
 */
int getCh();

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getChNoEcho
 *  Description: get a char from user NO echo NOR wait for Enter key to be pressed
 *  			 NON-CANONICAL MODE
 *  Returns: ascii decimal value of char, key code > 255 for function keys,
 *  or -1 for unkown key
 * =====================================================================================
 */
int getChNoEcho();


void read_terminfo_header( const char *, uint16_t * );
int get_str_cap( const char *ptr );
FILE *open_terminfo_file( );
size_t get_db_size( FILE * );
int get_code_from_ti( const char * );
const char * get_key_name( int kcode );
#endif   /* ti.h */
