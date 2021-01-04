/*
 * =====================================================================================
 *
 *       Filename:  getch.h
 *
 *    Description: This implements DOS getch() which gets a char from the user and
 *                 not wait for the user to hit the Enter key (NON-CANONICAL)
 *                 getch(): gets input while ECHO
 *                 getche(): gets input without ECHO to screen
 *
 *        Version:  1.0
 *        Created:  11/11/2020 08:16:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees group
 *
 * =====================================================================================
 */

#ifndef __GETCH_H__
#define __GETCH_H__
#include <stdbool.h>

#ifndef DEL
#define DEL 0x7f
#endif

#ifndef ESC
#define ESC 0x1b
#endif

/* The order is important */
static char *KNAMES[] = {
	"<F1>",
	"<F2>",
	"<F3>",
	"<F4>",
	"<F5>",
	"<F6>",
	"<F7>",
	"<F8>",
	"<F9>",
	"<F10>",
	"<F11>",
	"<F12>",
	"<INSERT>", /* INSRT */
	"<DEL>",    /* DEL   */
	"<PgUP>",   /* PgUp  */
	"<PgDN>",   /* PgDn  */
	"<UP>",     /* UP    */
	"<DN>",     /* DN    */
	"<FWD>",    /* FWD   */
	"<BWD>",    /* BWD   */
	"<END>",    /* END   */
	"<HOME>",   /* HOME  */
};

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getCh
 *  Description: get a char from user and not wait wait for Enter key to be pressed
 *  			 NON-CANONICAL MODE
 *  Returns: ascii decimal char value of key, or returns a code form 265 to 286
 *			 for keys F2 - F12, Home or arrows PgUp PgDn ...
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
 *  Returns: ascii decimal value of char or -1 for special function keys
 * =====================================================================================
 */
int getChNoEcho();

/* ===  Methods to create keys table ================================================ */
void create_keys_tbl ( );
void print_keys_tbl ( );
void free_keys_tbl ( );
#endif
