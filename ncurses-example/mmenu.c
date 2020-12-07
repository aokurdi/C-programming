/*
 * =====================================================================================
 *
 *       Filename:  mmenu.c
 *
 *    Description: main menu using curses
 *
 *        Version:  1.0
 *        Created:  11/15/2020 08:19:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees group
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <panel.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h> // to convert ip address

/*  My headers */
#include "mmenu.h"
#include "ip_calc.h"
#include "getIP.h"

/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */
#define COLOR_GREY 8
#define COLOR_LGREY 9
#define COLOR_DGREY 10

/* ===  Naming Convention  =============================================================
 * Private Func: _my_function
 *  Global Vars: MY_VAR
 *   Local Vars: myLocalVar
 *    Functions: my_function
 *       Macros: MYMACRO_ or MYMACRO
 * ================================================================================== */

/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ################################ */
int MAX_Y = 0;
int MAX_X = 0;
int MID_Y;
int MID_X;

/* Wins & Panels */
WINDOW *MENU_WIN, *RESULT_WIN, *DEBUG_WIN;
PANEL *MENU_PAN, *RESULT_PAN, *DEBUG_PAN;

/* Choice menu */
char *choices[] = {
    "1. Get Broadcast address.",
    "2. Get Integer Eq. of IP.",
    "3. Get A.B.C.D format.",
    "4. Get network ID.",
    "5. Get Subnet cardinality.",
    "6. Check if IP belongs to a subnet.",
    "7. Print network topology.",
};

int n_choices = sizeof(choices) / sizeof(char *);
int highlight = 1;

/*
 * ===  MAIN  ==========================================================================
 *         Name:  main
 *  Description: Entery point
 * =====================================================================================
 */
int
main (int argc, char *argv[]) {
    int choice = 0;

    /*  In case Ctrl-c is chought clean up before term */
    signal (SIGINT, free_and_exit);
    signal (SIGTERM, free_and_exit);

    /* Initialie Curses */
    initscr ( );
    noecho  ( );
    cbreak  ( ); /* Don't buffer input, pass every thing */
    clear   ( );
    nonl    ( );   /* Enter treated as cr not nl */

    /* Prepairing color pairs */
    start_color ( );
    init_color  (COLOR_GREY, 620, 620,  620);   /* Light grey */
    init_color  (COLOR_LGREY, 680, 680, 680);   /*    Grey    */
    init_color  (COLOR_DGREY, 420, 420, 420);   /* Dark grey  */
    init_pair   (1, COLOR_WHITE, COLOR_BLUE);
    init_pair   (2, COLOR_GREY, COLOR_BLACK);
    init_pair   (3, COLOR_BLACK, COLOR_GREY);
    init_pair   (4, COLOR_LGREY, COLOR_GREY);
    init_pair   (5, COLOR_DGREY, COLOR_GREY);
    init_pair   (6, COLOR_RED,   COLOR_GREY);
    init_pair   (7, COLOR_BLACK, COLOR_BLUE);

    /* Put window in middle of screen */
    getmaxyx (stdscr, MAX_Y, MAX_X);
    MID_Y = (MAX_Y - HT) / 4;
    MID_X = (MAX_X - WD) / 2;

    /* Prepare main menu and result windows */
    MENU_WIN   = newwin (HT, WD, MID_Y, MID_X);
    RESULT_WIN = newwin (HT, WD, getbegy (MENU_WIN) + 16, getbegx (MENU_WIN));
    DEBUG_WIN  = newwin (5, 40, getbegy (RESULT_WIN) + 7 , getbegx (RESULT_WIN) + 6);

    /* I want to use arrow keys in my wins*/
    keypad (MENU_WIN, TRUE);
    keypad (RESULT_WIN, TRUE);

    /* Set windows colors */
    bkgd  (COLOR_PAIR(1));
    wbkgd (MENU_WIN, COLOR_PAIR(3));
    wbkgd (DEBUG_WIN, COLOR_PAIR(3));
    wbkgd (RESULT_WIN, COLOR_PAIR(3));

    /* Drawing the windows */
    print_stdscr ( );
    print_menu (MENU_WIN, highlight);

    /* Attach panels to wins */
    MENU_PAN   = new_panel (MENU_WIN);
    RESULT_PAN = new_panel (RESULT_WIN);
    DEBUG_PAN  = new_panel (DEBUG_WIN);
    draw_debug_win ( );
    hide_panel (DEBUG_PAN);  // Hide debug win
    hide_panel (RESULT_PAN);  // Hide result win

    /* Show everything */
    update_panels ( );
    doupdate ( );

    do {
        choice = menu_loop ( );
        switch (choice) {
            case 1:
                print_result (RESULT_WIN, "  Get Broadcast Address  ", choice);
                break;
            case 2:
                print_result (RESULT_WIN, "  Get Integer Eq. of IP  ", choice);
                break;
            case 3:
                print_result (RESULT_WIN, "  Convert from int to A.B.C.D  ", choice);
                break;
            case 4:
                print_result (RESULT_WIN, "  Get Network ID  ", choice);
                break;
            case 5:
                print_result (RESULT_WIN, "  Get Subnet cardinality ", choice);
                break;
            case 6:
                print_result (RESULT_WIN, "  Check if IP is in Subnet  ", choice);
                break;
            case 7:
                print_result (RESULT_WIN, "  network topology  ", choice);
                break;
            case KEY_RESIZE:
                winch_hndlr (KEY_RESIZE);
                break;
            default:
                break;
        }
        wmove (MENU_WIN, 0, 0);
    } while (choice != -1);

    free_and_exit (0);
    return EXIT_SUCCESS;
} /* ----------  end of function main  ---------- */


/*
 * ===  Free mem and Exit prog  ========================================================
 *         Name:  free_and_exit
 *  Description: Free mem and exit
 * =====================================================================================
 */
void
free_and_exit (int sig) {

    /* Free mem and clean up */
    del_panel (RESULT_PAN);
    del_panel (DEBUG_PAN);
    del_panel (MENU_PAN);
    delwin (RESULT_WIN);
    delwin (DEBUG_WIN);
    delwin (MENU_WIN);
    endwin ( );

    /* Handling Intrupt and kill */
    if (sig == SIGINT || sig == SIGTERM) {
        fprintf (stderr, "Program recieved %s\n", (sig == SIGINT ? "SIGINT" : "SIGTERM"));
        exit (128 + sig);
    }
    return ;
}		/* -----  end of function exit  ----- */

/* ==========  Functions Section  =================================================== */

/*
 * ===  PRINT MAIN WIN  ================================================================
 *         Name:  print_stdscr
 *  Description: Sets background color and draw box on stdscr
 * =====================================================================================
 */
void
print_stdscr ( ) {
    box (stdscr, 0, 0);
    return;
} /* -----  end of function stdscr_win  ----- */

/*
 * ===  HANDLE WIN SIZE CHANGE  ========================================================
 *         Name:  sig_handler
 *  Description: when term is resized, this should redraw all windows
 * =====================================================================================
 */
void
winch_hndlr (int sig) {
    int isResultHidden, isDebugHidden;

    /* save result panel status */
    isResultHidden = panel_hidden (RESULT_PAN);
    isDebugHidden = panel_hidden (DEBUG_PAN);

    /* Clear stdscr */
    clear ( );

    /* Get new size */
    getmaxyx (stdscr, MAX_Y, MAX_X);
    MID_Y = (MAX_Y - HT) / 4;
    MID_X = (MAX_X - WD) / 2;

    /* Move the window and redraw */
    print_stdscr ( );
    move_panel (MENU_PAN, ((MAX_Y - HT) / 4), ((MAX_X - WD) / 2));
    draw_win_shadow (MENU_WIN);
    move_panel (RESULT_PAN, (getbegy (MENU_WIN) + 16), getbegx (MENU_WIN));
    move_panel (DEBUG_PAN, getbegy (RESULT_WIN) + 7, getbegx (RESULT_WIN) + 6);

    if (!isResultHidden) {
        show_panel (RESULT_PAN);
        draw_win_shadow (RESULT_WIN);
    } if (!isDebugHidden)
    top_panel (DEBUG_PAN);

    update_panels ( );
    doupdate ( );
} /* -----  end of function sig_handler  ----- */

/*
 * ===  PRINT MENU  ====================================================================
 *         Name:  print_menu
 *  Description: Prints the Main Menu
 * =====================================================================================
 */
void
print_menu (WINDOW *MENU_WIN, int is_highlight) {
    int x, y, i;

    x = 4;
    y = 3; /* Padding the box */
    draw_win_borders (MENU_WIN);
    draw_win_shadow  (MENU_WIN);

    wattron   (MENU_WIN, A_ITALIC|A_BOLD);
    mvwprintw (MENU_WIN, 0, 3, "  Main Menu  "); /* Printing box title */
    wattroff  (MENU_WIN, A_ITALIC|A_BOLD);

    /* Loop and print choices inside the box */
    for (i = 0; i < n_choices; ++i) {
        if (is_highlight == i + 1) {
            wattron   (MENU_WIN, A_REVERSE);
            mvwprintw (MENU_WIN, y, x, "%s", choices[i]);
            wattroff  (MENU_WIN, A_REVERSE);
        } else
            mvwprintw (MENU_WIN, y, x, "%s", choices[i]);
        y++;
    }

    mvwprintw (MENU_WIN, 11, 14, "Press F12 or Q to quit");
    mvwchgat  (MENU_WIN, 11, 20, 3, A_COLOR, 6, NULL); /* Changing F12 color to red */
    mvwchgat  (MENU_WIN, 11, 27, 1, A_COLOR, 6, NULL); /* Changing Q color to red   */

    /* Show the Box */
    update_panels ( );
    doupdate ( );
    return;
} /* -----  end of function print_menu  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  draw_win_borders
 *  Description: Prints borders around wins
 * =====================================================================================
 */
void
draw_win_borders (WINDOW *win)
{
    /* Outer Border  */
    /* Shadoow lines */
    wattron (win, COLOR_PAIR(5));
    box (win, 0, 0);
    wattroff (win, COLOR_PAIR(5));

    /* Light Lines */
    wattron  (win, COLOR_PAIR(4));
    mvwaddch (win, 0, 0, ACS_ULCORNER);
    mvwvline (win, 1, 0, 0, 12);
    mvwaddch (win, HT - 1, 0, ACS_LLCORNER);
    mvwhline (win, 0, 1, 0, WD - 2);

    /* Inner Borders       */
    /*  Light Border lines */
    mvwaddch (win, 1, WD - 3, ACS_URCORNER);
    mvwvline (win, 2, WD-3, 0, 10);
    mvwaddch (win, HT - 2, WD - 3, ACS_LRCORNER);
    mvwhline (win, HT-2, 3, 0, WD - 6);
    wattroff (win, COLOR_PAIR(4));

    /* Shadow Border lines */
    wattron  (win, COLOR_PAIR(5));
    mvwhline (win, 1, 3, 0, WD - 6);
    mvwaddch (win, 1, 2, ACS_ULCORNER);
    mvwvline (win, 2, 2, 0, 11);
    mvwaddch (win, HT - 2, 2, ACS_LLCORNER);
    wattroff (win, COLOR_PAIR(5));

    return;
} /* -----  end of function draw_win_borders  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  draw_win_sahdow
 *  Description: Draws shadow around a win
 *  shadow is drawn on the stdscr
 * =====================================================================================
 */
void
draw_win_shadow (WINDOW *win)
{
    int y, x;
    getbegyx (win, y, x);
    attron (COLOR_PAIR(7));
    for (int i = (x + 2); i < (x + WD + 1); i++)
        mvaddch ((y + HT), i, ACS_CKBOARD);

    for (int i = (y + 1); i < (y + HT + 1); i++) {
        mvaddch (i, (x + WD), ACS_CKBOARD);     /* Vertical is half wide */
        mvaddch (i, (x + WD + 1), ACS_CKBOARD); /* need to print dble    */
    }
    attroff(COLOR_PAIR(7));
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  del_win_sahdow
 *  Description: delets the shadow
 * =====================================================================================
 */
void
del_win_shadow (WINDOW *win) {
    int y, x;

    getbegyx (win, y, x);
    attron (COLOR_PAIR(1));
    for (int i = (x + 2); i < (x + WD + 1); i++)
        mvaddch ((y + HT), i, ' ');

    for (int i = (y + 1); i < (y + HT + 1); i++) {
        mvaddch (i, (x + WD), ' ');
        mvaddch (i, (x + WD + 1), ' ');
    }
    attroff (COLOR_PAIR(1));
}

/*
 * ===  PRINT RESULT BOX  ==============================================================
 *         Name:  print_result
 *  Description: Draw the results menu
 * =====================================================================================
 */
void
print_result (WINDOW *win, char *title, int choice) {

    unsigned int ipAddr, mask, binMask, bCast, netId;

    /* Setting up borders shadow and colors */
    wbkgd (win, COLOR_PAIR(3));
    draw_win_borders (win);
    draw_win_shadow (win);

    /* Printin title lable */
    wattron   (win, A_BOLD|A_ITALIC);
    mvwprintw (win, 0, 3, "%s", title);
    wattroff  (win, A_BOLD|A_ITALIC);

    /* Echo, show cursor, and Panel */
    echo ( );
    curs_set (1);
    show_panel (RESULT_PAN);
    update_panels ( );
    doupdate ( );

    /* move cursor to get input from user */
    wmove (win, 1, 2);

    /* Depending on user choice we select operations */
    if (choice != 5 && choice != 3) {
        ipAddr = get_ip (win, DEBUG_PAN);
    }

    if (choice != 2 && choice != 3) {
        mask = get_mask (win);
        setup_mask (mask, &binMask);
    }

    if (choice == 1) {
        get_broadcast_address (&ipAddr, &binMask, &bCast);
        wmove (win, 3, 2);
        print_broadcast (win, &bCast);
    }

    if (choice == 2)
        print_integer_format (win, &ipAddr);

    if (choice == 3)
        print_ABCD_format (win);

    if (choice == 4) {
        get_network_id   (&ipAddr, &binMask, &netId);
        print_network_id (win, &netId);
    }

    if (choice == 5) {
        mvwhline  (win, 3, 4, 0, 44);
        mvwprintw (win, 4, 4, "Max Hosts/Net: %u", get_subnet_cardinality(&binMask));
    }

    if (choice == 6) {
        wmove (win, 3, 2);
        netId = get_ip (win, DEBUG_PAN);
        mvwhline (win, 5, 4, 0, 44);

        if (is_in_subnet (&netId, &ipAddr, &binMask))
            mvwprintw (win, 6, 4, "IP is member of Subnet");
        else {
            wattron   (win, A_BLINK | COLOR_PAIR(6));
            mvwprintw (win, 6, 4, "IP is NOT member of Subnet");
            wattroff  (win, A_BLINK | COLOR_PAIR(6));
        }
    }

    if (choice == 7) {
        get_topology (win, &ipAddr, &binMask);
    }

    update_panels ( );
    doupdate ( );

    /* Clear window and hide it */
    clear_result_win (win);
    return;
} /* -----  end of function get_choice  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  draw_DEBUG_WIN
 *  Description: Draws debug win
 * =====================================================================================
 */
void
draw_debug_win ( ) {
    int y, x;

    getyx (DEBUG_WIN, y, x);

    wattron  (DEBUG_WIN, COLOR_PAIR(5));
    mvwhline (DEBUG_WIN, 0, 0, ACS_HLINE, 38);
    mvwaddch (DEBUG_WIN, 0, 0, ACS_ULCORNER);
    mvwvline (DEBUG_WIN, 1, 0, 0, 2);
    mvwaddch (DEBUG_WIN, 3, 0, ACS_LLCORNER);
    wattroff (DEBUG_WIN, COLOR_PAIR(5));

    wattron  (DEBUG_WIN, COLOR_PAIR(4));
    mvwhline (DEBUG_WIN, 3, 1, 0, 38);
    mvwvline (DEBUG_WIN, 1, 37, 0, 2);
    mvwaddch (DEBUG_WIN, 0, 37, ACS_URCORNER);
    mvwaddch (DEBUG_WIN, 3, 37, ACS_LRCORNER);
    wattroff (DEBUG_WIN, COLOR_PAIR(4));

    wattron   (DEBUG_WIN, A_BOLD|A_ITALIC);
    mvwprintw (DEBUG_WIN , 0, 9, " Debug: For fun!! ");
    wattroff  (DEBUG_WIN, A_BOLD|A_ITALIC);

    for (int i = (x + 2); i < (40); i++)
        mvwaddch (DEBUG_WIN, (y + 4), i, ACS_CKBOARD);

    for (int i = (y + 1); i < (y + HT + 1); i++) {
        mvwaddch (DEBUG_WIN, i, (38), ACS_CKBOARD);
        mvwaddch (DEBUG_WIN, i, (39), ACS_CKBOARD);
    }
    show_panel (DEBUG_PAN);
    top_panel (DEBUG_PAN);
    update_panels ( );
    return ;
}		/* -----  end of function draw_DEBUG_WIN  ----- */

/*
 * ===  CLEAR RESULT WIN  ==============================================================
 *         Name:  clear_RESULT_WIN
 *  Description: clears and dim the result box
 * =====================================================================================
 */
void
clear_result_win (WINDOW *win) {
    int tmp;

    mvwprintw (win, 12, 8, " <-_Press space bar to continue_-> ");
    update_panels ( );
    doupdate ( );

    noecho ( );
    while ((tmp = getch( )) != ' ') {
        if (tmp == KEY_RESIZE)
            winch_hndlr (KEY_RESIZE);
    }

    // Clear and hide panel
    wclear (win);
    del_win_shadow (RESULT_WIN);
    hide_panel (RESULT_PAN);
    hide_panel (DEBUG_PAN);
    update_panels( );
    doupdate( );

    return;
} /* -----  end of function clear_RESULT_WIN  ----- */

/*
 * ===  MENU LOOP  =====================================================================
 *         Name:  menu_loop
 *  Description:
 * =====================================================================================
 */
int
menu_loop ( ) {
    int ch, choice = 0;

    /* Hide curser */
    curs_set (0);

    /* Main menu loop */
    while (TRUE) {
        ch = wgetch (MENU_WIN);
        switch (ch) {
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    highlight++;
                break;
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    highlight--;
                break;
            case ENTR:
                choice = highlight;
                break;
            case KEY_F(12):
                choice = -1;
                break;
            case 'Q':
                choice = -1;
                break;
            case 'q':
                choice = -1;
                break;
            case KEY_RESIZE:
                winch_hndlr (KEY_RESIZE);
                break;
            default:
                break;
        }

        print_menu (MENU_WIN, highlight);
        if (choice != 0)
            break;
    }
    return choice;
} /* -----  end of function menu_loop  ----- */
