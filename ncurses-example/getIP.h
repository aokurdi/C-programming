/*
 * =====================================================================================
 *
 *       Filename:  getIP.h
 *
 *    Description: get an IPv4 ip addr from a user (using curses lib)
 *
 *        Version:  1.0
 *        Created:  11/19/2020 11:56:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees group
 *
 * =====================================================================================
 */

#ifndef __GETIP_H__
#define __GETIP_H__

#ifndef DEL
#define DEL 0x14a
#endif

#ifndef BKS
#define BKS 0x107
#endif

#ifndef ESC
#define ESC 0x1b
#endif

#ifndef ENTR
#define ENTR 0x0d
#endif

#ifndef IPLEN
#define IPLEN 16
#endif

/*
 * ===  get_ip  ========================================================================
 *         Name:  void get_ip (WINDOW *win, char *ipbuf)
 *  Description: gets an IPv4 format string  from user
 *  PARAMS:
 *  win: Curses window to get from and write to
 *  debugPan: This is a window that shows who status of third digit is determined
 *  Return: a positive number that represents the numeric value of an IPv4 addr
 * =====================================================================================
 */
unsigned int
get_ip(WINDOW *win, PANEL *debugPan);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_mask
 *  Description: gets network mask from user, mask should be 1 - 32
 *  Return: mask value in int format
 * =====================================================================================
 */
unsigned int
get_mask(WINDOW *win);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_third_digit
 *  Description: get the last digit in part IP depending on status of previous digits
 *  PARAMS:
 *  int status: 1 restrect last digit to <= 5, and 2 no restrection
 *  Return: -1 if user hits backspace, 0 - 9 if successfuly got third, Or ENTR
 *           if user not needs third digit
 * =====================================================================================
 */
int
get_third_digit (WINDOW *win, int status, bool islast);
#endif
