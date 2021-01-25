/*
 * =====================================================================================
 *
 *       Filename:  ip_calc.c
 *
 *    Description: claculate network stuff like finding a broadcast address
 *                 and networkId and subnetting etc.
 *
 *        Version:  1.0
 *        Created:  20/12/2019 08:16:13 PM
 *       Revision:  none
 *       Compiler:  gcc or clang
 *
 *         Author:  Ayman Kurdi (AK), a.o.kurdi2@gmail.com
 *   Organization:  Arees group
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <panel.h>
#include <string.h>
#include <arpa/inet.h>

#include "ip_calc.h"

/* #####   MACROS  -  LOCAL TO THIS SOURCE FILE   ################################### */
#ifndef IPLEN
#define IPLEN 16 /* Max ip addr length + 1 */
#endif

#ifndef MASKLEN
#define MASKLEN 32
#endif

#ifndef ENTR
#define ENTR 0x0d
#endif


/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ################################ */
static int MASK;

/*
 * ===  Convert to a bin mask  =========================================================
 *        Name:  setup_mask
 * discreption: Function to convert a mask from a decimal value
 *              to a network binary
 *
 * Params:
 * mask: the decimal value of the mask
 * dest: a pointer to a destination to store the
 *       binary mask.
 *
 * =====================================================================================
 */
void
setup_mask(int mask, unsigned int *dest) {
    int pos;
    unsigned int temp = 0x0;

    MASK = mask; /* keep a copy of numeric mask for use in printing */

    /* Sitting mask bits, this way i don't need to add trailing 0's */
    for (pos = MASKLEN; pos > (MASKLEN - mask); --pos)
        temp |= (1 << (pos - 1)); /* Push 1's starting from MSB */

    *dest = temp; /* Avoid using pointer in a loop (DANGEROUS) */
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_subnet_cardinality
 *  Description: determines the maximum number of hosts in a subnet from a mask value
 *
 * Params:
 * mask: pointer to an int holding the decimal value of the mask
 * Return: returns the number of hosts in a subnet
 *
 * =====================================================================================
 */
unsigned int
get_subnet_cardinality(unsigned int *mask) {

    unsigned int comp_mask = ~*mask;
    if (comp_mask < 2)
        return 1;
    return (comp_mask - 1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_network_id
 *  Description: determines the network ID from an IPv4 and a mask
 *               Function to calculate the network ID by applying
 *               a bitwise '&' on a given ip address and a given mask
 *               and store the netID in a dist pointed to as dist
 * NOTE: ip_address will be converted to Net Long format
 *
 * Params:
 * ipAddr: a pointer to the ip address source (Host Long)
 * mask: a pointer to the mask source
 * dest: a pointer to the destination to store the result (Net Long)
 *
 * =====================================================================================
 */
void
get_network_id(unsigned int *ipAddr, unsigned int *mask, unsigned int *dest) {

    /* convert to net long (Big Endian); */
    *ipAddr = htonl(*ipAddr);

    /* get net id and store it in dest */
    *dest = *ipAddr & *mask;
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_network_id
 * Description: Function to print the network ID in A.B.C.D format
 *
 * params:
 * network_id: a pointer to the network ID
 *             it must be in  Net Long format (Big Endian)
 *
 * =====================================================================================
 */
void
print_network_id(WINDOW *win, unsigned int *networkId) {

    char netIdStr[IPLEN];
    unsigned int netId, y, x;

    getyx(win, y, x);
    x = 4;

    /* convert from network long to host long (little Endian) */
    netId = ntohl(*networkId);

    /* Convert from integral fromat to A.B.C.D string */
    inet_ntop(AF_INET, &netId, netIdStr, IPLEN);
    mvwhline(win, y + 1, x, 0, 44);
    mvwprintw(win, y + 2, x, "Network: %s/%d", netIdStr, MASK);
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_broadcast_address
 * Description: This function calculates the broadcast value by
 * converting a given mask value to a complemntary mask and
 * applying a bitwise or on the comp_mask with a given IPv4 addr
 *
 * params:
 * ipAddr: pointer to the ip address source (Host Long)
 * mask   : pointer to the mask source
 * dest: the destenation to store the broadcast value in (net long format).
 *
 * =====================================================================================
 */
void
get_broadcast_address(unsigned int *ipAddr, unsigned int *mask, unsigned int *dest) {

    unsigned int compMask, networkId;

    networkId = compMask = 0x0; /* initialize               */
    *dest = 0x0;                /* remove garbage from dist */

    /* get network ID to calculate the brdcast */
    get_network_id(ipAddr, mask, &networkId);

    /* Get the mask complemntary to calc brdcast */
    compMask = ~*mask;

    /* Broadcast is calculated by OR with comp_mask */
    *dest = networkId | compMask;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_broadcast_addr
 * Description: Function to print the broadcast addr in A.B.C.D fromat
 *
 * @Paramas:
 * broadcast_addr: the address to be printed, it must be
 *                 in net long format (big endian);
 *
 * =====================================================================================
 */
void
_print_broadcast_addr(WINDOW *win, unsigned int *brdcastAddr, char const *callerFunc) {

    char brdcastStr[IPLEN];
    int y, x;
    unsigned int bAddr = ntohl(*brdcastAddr);

    inet_ntop(AF_INET, &bAddr, brdcastStr, IPLEN);

    getyx(win, y, x);
    x = 4;
    if (0 == strcmp(callerFunc, "print_result"))
    {
        mvwhline(win, y + 1, x, 0, 44);
        y++;
    }
    mvwprintw(win, y + 1, x, "Broadcast address: %s", brdcastStr);
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name: print_interger_format
 * Description: Function to print ip address in integeral format
 *
 * Params:
 * ip_addr: the ip address to be printed (little endian)
 *
 * =====================================================================================
 */
void print_integer_format(WINDOW *win, unsigned int *ipAddr) {

    int x, y;
    getyx(win, y, x);
    x = 4;
    /* convert ip to Big endian */
    *ipAddr = htonl(*ipAddr);
    mvwhline(win, y + 1, x, 0, 44);
    mvwprintw(win, y + 2, x, "IP equevelant Int value is: %u", *ipAddr);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_ABCD_Format
 * Function to print ip address in A.B.C.D format
 *
 * =====================================================================================
 */
void
print_ABCD_format(WINDOW *win) {

    char buffer[IPLEN], ch;
    int i, y, x;
    unsigned int ipAddr;

    getyx(win, y, x);
    x = 4;
    mvwprintw(win, y + 1, x, "Enter ip address (INT FROMAT): ");

    for (i = 0; i < 10; i++) {
        while (!isdigit(ch = wgetch(win))) {
            if (ch == ENTR && i) {
                ch = '\0';
                break;
            } else if (ch == 0x07) {
                if (i) {
                    wprintw (win, "\b \b");
                    --i;
                }
            }
        }

        buffer[i] = ch;
        if (ch == '\0')
            break;
        else
            waddch (win, ch);
    }

    ipAddr = atoi(buffer);
    ipAddr = htonl(ipAddr);

    /* using inet_pton from arpa/net to convert ip to integer */
    inet_ntop(AF_INET, &ipAddr, buffer, IPLEN);
    mvwhline(win, y + 2, x, 0, 44);
    mvwprintw(win, y + 3, x, "The IP in A.B.C.D fromat is: %s", buffer);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  is_in_subnet
 * Description: Function to check if a given ip addr belongs to th esubnet
 *
 * Params:
 * networkId: the network id (Host long) will be converted to Net long
 * ipAddr: the ip address to be checked (Host Long) will be converted
 * mask: pointer to the subnet mask
 * returns: 1 if it belongs to subnet, 0 if it dos't
 *
 * =====================================================================================
 */
int
is_in_subnet(unsigned int *networkId, unsigned int *ipAddr, unsigned int *mask) {

    unsigned int testNetworkId;

    /*
     *     apply the mask on the ip addr to check which
     *     network it belongs to.
     */
    get_network_id(ipAddr, mask, &testNetworkId);
    *networkId = htonl(*networkId);

    if (testNetworkId == *networkId)
        return 1;
    else
        return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_topology
 *  Dsecription: prints the entire network topology by applying a mask on an ip addr
 *               of any host that belongs to the networ
 *
 * Params:
 * ipAddr: a pointer to an int holding an ip address of a host in the network
 *   mask: the mask value of the network
 *
 * =====================================================================================
 */
void
get_topology(WINDOW *win, unsigned int *ipAddr, unsigned int *mask) {

    char firstIpStr[IPLEN], lastIpStr[IPLEN];
    unsigned int networkId, brdcastAddr, compMask, firstIp, lastIp;
    int y, x;

    get_network_id(ipAddr, mask, &networkId);
    compMask = ~*mask;
    brdcastAddr = networkId | compMask;

    firstIp  =   networkId + 1;
    firstIp  =  ntohl(firstIp);
    lastIp   = brdcastAddr - 1;
    lastIp   =   ntohl(lastIp);

    inet_ntop(AF_INET, &firstIp, firstIpStr, IPLEN);
    inet_ntop(AF_INET, &lastIp, lastIpStr, IPLEN);

    getyx(win, y, x);
    print_network_id(win, &networkId);
    getyx(win, y, x);
    x = 4;
    mvwprintw(win, y + 1, x, "Max Hosts/Net: %u", get_subnet_cardinality(mask));
    mvwprintw(win, y + 2, x, "The first host ip is: %s", firstIpStr);
    mvwprintw(win, y + 3, x, "The last  host ip is: %s", lastIpStr);
    print_broadcast(win, &brdcastAddr);
}
