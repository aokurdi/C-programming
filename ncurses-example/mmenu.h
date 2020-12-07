/*
 * =====================================================================================
 *
 *       Filename:  mmenu.h
 *
 *    Description: Header for main menu
 *
 *        Version:  1.0
 *        Created:  11/18/2020 08:46:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees group
 *
 * =====================================================================================
 */
#ifndef __MMENU_H__
#define __MMENU_H__

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

#define KEY_ESC 0x1b

#define WD 52
#define HT 14

/*
 * ===  Prints the main menu  ==========================================================
 *         Name:  print_menu
 *  Description: prints the main menu
 * =====================================================================================
 */
    void
print_menu (WINDOW *menu_win, int highlight);

/*
 * ===  Main loop  =====================================================================
 *         Name:  menu_loop
 *  Description: The main loop where user selects a choice
 * =====================================================================================
 */
    int
menu_loop ( );

/*
 * ===  Handles term resize  ===========================================================
 *         Name:  sig_handler
 *  Description: handles terminal resize signal
 * =====================================================================================
 */
    void
winch_hndlr (int sig);

/*
 * ===  Prin the result win  ===========================================================
 *         Name:  print_result
 *  Description: Draw the results menu
 * =====================================================================================
 */
    void
print_result (WINDOW *win, char *title, int choice);

/*
 * ===  CLEAR RESULT WIN  ==============================================================
 *         Name:  clear_result_win
 *  Description: clears and dim the result box
 * =====================================================================================
 */
    void
clear_result_win (WINDOW *win);

/*
 * ===  Draw win border  ===============================================================
 *         Name:  draw_menu_borders
 *  Description: Draws borders around wins
 * =====================================================================================
 */
    void
draw_win_borders (WINDOW *win);

/*
 * ===  Draws shadow around wins  ======================================================
 *         Name:  draw_win_shadow
 *  Description: Draws a border around a win
 * =====================================================================================
 */
    void
draw_win_shadow (WINDOW *win);

/*
 * ===  Delete Shadow  =================================================================
 *         Name:  del_win_shadow
 *  Description: Deletes shadow around wins
 * =====================================================================================
 */

    void
del_win_shadow (WINDOW *win);

/*
 * ===  PRINT MAIN WIN  ================================================================
 *         Name:  print_stdscr
 *  Description: Sets background color and draw box on stdscr
 * =====================================================================================
 */
    void
print_stdscr ( );

/*
 * ===  Free mem and Exit  =============================================================
 *         Name:  exit
 *  Description: Free mem and exit 
 * =====================================================================================
 */
    void
free_and_exit (int sig);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  draw_debug_win
 *  Description: Draws debug win
 * =====================================================================================
 */
    void
draw_debug_win ( );

#endif
