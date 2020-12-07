/*
 * =====================================================================================
 *
 *       Filename:  getIP.c
 *
 *    Description: gets an IPv4 address from a user (using curses lib)
 *
 *        Version:  1.0
 *        Created:  11/19/2020 11:51:51 PM
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
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "getIP.h"
#include "mmenu.h"

/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */

/* Macro to convert to binary */
#define BINSTR "0b%c%c%c%c%c%c%c%c"
#define TOBIN_(num)          \
    (num & 0x80 ? '1' : '0'),\
    (num & 0x40 ? '1' : '0'),\
    (num & 0x20 ? '1' : '0'),\
    (num & 0x10 ? '1' : '0'),\
    (num & 0x08 ? '1' : '0'),\
    (num & 0x04 ? '1' : '0'),\
    (num & 0x02 ? '1' : '0'),\
    (num & 0x01 ? '1' : '0')

/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ##################### */

/*
 * ===  Gets part of IP  ===============================================================
 *         Name:  get_part_ip
 *  Description: gets 3 digits of ip addr
 *
 *  Notes: using int to grab char from keyboard is importan to catch special chars
 *  for example, Backspace holds the value of 0x107 when using curses and DEL holds
 *  0x14A and if char is used it can only store values of -127 to 128
 *  IMPORTANT: This routine should not be used directly, it is called from get_ip()
 *  Params:
 *     win: curses window to get input from
 *  partIP: buffer to store the part of the IP address
 *  islast: if last part of IP addr, we should disabel '.'
 *    dwin: debug window to show how third didgit status is proccessed
 * =====================================================================================
 */
static void
get_part_ip (WINDOW *win, char *partIP, bool islast, WINDOW *dwin) {

    int i = 0, tmp, y, x;
    short thirdStatus = 0;
    getyx(win, y, x);

    /*
     * No IPv4 should exceed the value of 255
     * so i have to keep track of the first and
     * second digits to either enable the third
     * digit or limit it to 5 or disable it
     */

_loop:               /* Goto is useful sometimes!! */
    for (; i < 3; i++) {
        switch (i) {
            case 0:
                while (!isdigit(tmp = wgetch(win))) {
                    if (tmp == KEY_RESIZE)
                        winch_hndlr(KEY_RESIZE);
                    else if (tmp == BKS || tmp == ENTR) /* ignore backspace   */
                        wmove(win, y, x);
                    else if (isprint(tmp))              /* erase any alphabet */
                        wprintw(win, "\b \b");
                }
                partIP[i] = tmp;    /* Now it's ok to store it in buffer */

                /* Prepare thir digit status */
                thirdStatus ^= thirdStatus; /* Zero it        */
                if (tmp == '2')             /* 1st digit == 2 */
                    thirdStatus = 1 << 2;   /* 0b0100         */
                else if (tmp > '2')         /* 1st digit > 2  */
                    thirdStatus = 1 << 3;   /* 0b1000         */

                partIP[i + 1] = '\0'; /* terminate the partIPing in case only 1 digit needed */
                mvwprintw (dwin, 1, 2, "3rd digit status: "BINSTR, TOBIN_(thirdStatus));
                mvwprintw (dwin, 2, 2, "3rd digit status: %hd", thirdStatus);
                update_panels ( );
                doupdate ( );
                break;

            case 1: /* get second digit */
                while (!isdigit(tmp = wgetch(win))) {
                    if (tmp == KEY_RESIZE)
                        winch_hndlr(KEY_RESIZE);
                    else if (tmp == BKS) {
                        wprintw(win, " \b"); /* if user decided to erase first digit */
                        i--;                 /* reset the loop */
                        goto _loop;
                    } else if (tmp == ENTR) {
                        partIP[i] = '\0';
                        goto _end;
                    } else if (tmp == '.') {
                        if (!islast)
                            goto _end;
                        else
                            wprintw (win, "\b \b");
                    }
                    else if (isprint(tmp)) /* Erase alphbets */
                        wprintw(win, "\b \b");
                }
                partIP[i] = tmp;

                /* setting up 3rd digit status: < 5, == 5, > 5      */
                thirdStatus &= 0x0C;       /* clear LSB 2 bits only */
                if (tmp == '5')            /* 2nd digit == 5        */
                    thirdStatus += 1 << 0; /* 0xXX01                */
                else if (tmp > '5')        /* 2nd digit > 5         */
                    thirdStatus += 1 << 1; /* 0xXX10                */

                partIP[i + 1] = '\0'; /* Terminate partIPing */
                mvwprintw (dwin, 1, 2, "3rd digit status: "BINSTR, TOBIN_(thirdStatus));
                mvwprintw (dwin, 2, 2, "3rd digit status: %hd", thirdStatus);
                update_panels ( );
                doupdate ( );
                break;

            case 2:
                tmp = get_third_digit (win, thirdStatus, islast);
                if (tmp == -1) {  /* Delete last digit */
                    i--;
                    goto _loop;
                } else if (tmp == ENTR) {
                    partIP[i] = '\0';
                    break;
                } else {
                    partIP[i] = tmp;
                    partIP[i+1] = '\0';
                }
                break;
        }
    }

_end:
    mvwprintw (dwin, 1, 2, "3rd digit status: 0b00000000");
    mvwprintw (dwin, 2, 2, "3rd digit status: 0");
    update_panels ( );
    doupdate ( );

    if (!islast)
        mvwaddch(win, y, x + strlen(partIP), '.');

    /* First digit must be greater than zero or "inet_pton" fails */
    if (partIP[0] == '0' && partIP[1] != '\0')
        for (int i = 0; i < 4; i++)
            partIP[i] = partIP[i+1];

    return;
} /* -----  end of function get_part_ip  ----- */


/*
 * ===  Gets the last digit  ============================================================
 *         Name:  get_third_digit
 *  Description: get the last digit in part IP depending on status of previous digits
 *  PARAMS:
 *  status: 1 restrect last digit to <= 5, and 2 no restrection
 *  Return: -1 if user hits backspace, 0 - 9 if successfuly got third, Or ENTR
 *           if user not needs third digit
 * =====================================================================================
 */
int
get_third_digit (WINDOW *win, int status, bool islast) {

    int third;

    switch (status) {
        case 0x6 ... 0x0A:     /* Third digit is disabled */
            do {
                third = wgetch(win);
                if (third == KEY_RESIZE) /* Handle term resize */
                    winch_hndlr(KEY_RESIZE);
                else if (third == '.' && !islast) {
                    wprintw (win, "\b \b");
                    return ENTR;
                } else if (isprint(third))
                    wprintw(win, "\b \b"); // Erase alphabets
            } while (third != BKS && third != ENTR);

            switch (third) {
                case BKS:
                    wprintw(win, " \b");
                    return -1;
                case ENTR:
                    return ENTR;
            }

        case 5:     /* third is restrected to <= 5 */
            while (!isdigit (third = wgetch (win)) || third > '5') {
                switch (third) {
                    case KEY_RESIZE: /* Handle term resize */
                        winch_hndlr(KEY_RESIZE);
                        break;
                    case BKS:
                        wprintw(win, " \b");
                        return -1;
                    case '.':
                        if (!islast)
                            return ENTR;
                        else
                            wprintw (win, "\b \b");
                        break;
                    case ENTR:
                        return ENTR;
                    default:
                        if (isprint(third))
                            wprintw(win, "\b \b"); /* Erase alphabets */
                        break;
                }
            }
            break;

        default :     /* Third fully enabled */
            while (!isdigit (third = wgetch (win))) {
                switch (third) {
                    case KEY_RESIZE: /* Handle term resize */
                        winch_hndlr(KEY_RESIZE);
                    case BKS:
                        wprintw(win, " \b");
                        return -1;
                    case ENTR:
                        return ENTR;
                    default:
                        if (isprint(third))
                            wprintw(win, "\b \b"); // Erase alphabets
                        break;
                }
            };
            break;
    }
    return third;
}		/* -----  end of function get_third_digit  ----- */


/*
 * ===  Gets the IP from a user  =======================================================
 *         Name:  void get_ip (WINDOW *win, char *ipBuf)
 *  Description: gets an IPv4 format string  from user
 *  PARAMS:
 *  win: Curses window to get from and write to
 *  debugPan: This is a window that shows who status of third digit is determined
 *  Return: a positive number that represents the numeric value of an IPv4 addr
 * =====================================================================================
 */
unsigned int
get_ip (WINDOW *win, PANEL *debugPan) {

    char ip_xxx[4] = "\0", ipBuf[IPLEN];
    unsigned int ipAddr = 0x0;
    int i, y, x;

    show_panel (debugPan);
    update_panels ( );
    doupdate ( );

    /* Get current curser position */
    getyx(win, y, x);

    /* depending from position determine what to print ;) */
    if (y < 3)
        mvwprintw(win, y+1, x+2, "Enter IPv4 addr: ");
    else
        mvwprintw(win, y+1, x+2, "Enter network id addr: ");

    /* Initialize buffer very important */
    for (i = 0; i < IPLEN; i++)
        ipBuf[i] = '\0';

    for (i = 0; i < 4; i++) {
        if (i == 3)     /* If last part */
            get_part_ip(win, ip_xxx, TRUE,  panel_window (debugPan));
        else
            get_part_ip(win, ip_xxx, FALSE, panel_window (debugPan));

        if (i == 0)
            strcpy(ipBuf, ip_xxx);
        else
            strcat(ipBuf, ip_xxx);

        if (i < 3)
            ipBuf[strlen(ipBuf)] = '.';
    }

    if (!inet_pton(AF_INET, ipBuf, &ipAddr))
        mvwprintw (win, 8, 4, "Erro! couldn't understand this ip");

    hide_panel (debugPan);
    return ipAddr;
} /* -----  end of function get_ip  ----- */

/* Throw compiler err if trying to call get_part_ip directly */
#define get_part_ip get_ip

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_mask
 *  Description: gets network mask from user, mask should be 1 - 32
 *  retursn: mask value in int format
 * =====================================================================================
 */
unsigned int
get_mask (WINDOW *win) {

    int i, y, x;
    unsigned int mask = 0;
    char ch[3];
    ch[2] = '\0';

    getyx(win, y, x);
    mvwprintw(win, y+1, 4, "Enter Network mask (1 - 32): ");
    getyx(win, y, x);

_loop:
    for (i = 0; i < 2; ++i) {
        switch (i) {
            case 0:
                while (!isdigit(mask = wgetch(win))) {
                    if (mask == KEY_RESIZE)
                        winch_hndlr(KEY_RESIZE);
                    if (mask == BKS || mask == ENTR)
                        wmove(win, y, x);
                    else if (isprint(mask))
                        wprintw(win, "\b \b");
                }
                ch[i] = mask;
                break;

            case 1:
                while (!isdigit(mask = wgetch(win))) {
                    if (mask == KEY_RESIZE)
                        winch_hndlr(KEY_RESIZE);
                    if (mask == BKS) {
                        wprintw(win, " \b");
                        goto _loop;
                    } else if (mask == ENTR) {
                        ch[i] = '\0';
                        goto _exit;
                        break;
                    }
                    else if (isprint(mask))
                        wprintw(win, "\b \b");
                }

                if (ch[0] == '3' && mask > '2') {
                    wprintw(win, "\b \b");
                    while (!isdigit(mask = wgetch(win)) || mask > '2') {
                        if (mask == KEY_RESIZE)
                            winch_hndlr(KEY_RESIZE);
                        if (mask == BKS) {
                            wprintw(win, " \b");
                            goto _loop;
                        }
                        else if (mask == ENTR) {
                            ch[i] = '\0';
                            break;
                        }
                        else if (isprint(mask)) wprintw(win, "\b \b");
                    }
                    ch[i] = mask;
                    break;
                } else if (ch[0] > '3') {
                    ch[i] = '\0';
                    wprintw(win, "\b \b");
                    while ((mask = wgetch(win)) != BKS && mask != ENTR) {
                        if (isprint(mask))
                            wprintw(win, "\b \b");
                        if (mask == KEY_RESIZE)
                            winch_hndlr(KEY_RESIZE);
                    }

                    if (mask == BKS) {
                        wprintw(win, " \b");
                        goto _loop;
                    } else if (mask == ENTR) {
                        ch[i] = '\0';
                        break;
                    }
                } else
                    ch[i] = mask;
                break;
        }
    }

_exit:
    mask = atoi(ch);
    return mask;
}
