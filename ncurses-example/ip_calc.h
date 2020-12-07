/**
 ** File: ip_calc.h
 ** Descreption: claculate internal network and subnetting
 **
 ** Author: Ayman Kurdi
 ** Date: 20 Dec 2019
 ** License: I don't care, take use do whatever.
 */
#ifndef _IP_CALC_H
#define _IP_CALC_H

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
setup_mask(int mask, unsigned int *dest);

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
get_subnet_cardinality(unsigned int *mask);

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
get_network_id(unsigned int *ipAddr, unsigned int *mask, unsigned int *dest);

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
print_network_id(WINDOW *win, unsigned int *networkId);

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
get_broadcast_address(unsigned int *ipAddr, unsigned int *mask, unsigned int *dest);

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
_print_broadcast_addr(WINDOW *win, unsigned int *brdcastAddr, char const *callerFunc);
/* Macro to get the function name param inculuded auto */
#define print_broadcast(win, brdcastAddr) _print_broadcast_addr(win, brdcastAddr, __func__)

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
void
print_integer_format(WINDOW *win, unsigned int *ipAddr);

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
is_in_subnet (unsigned int *networkId, unsigned int *ipAddr, unsigned int *mask);

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_ABCD_Format
 * Function to print ip address in A.B.C.D format
 *
 * =====================================================================================
 */
void
print_ABCD_format(WINDOW *win);

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
get_topology(WINDOW *win, unsigned int *ipAddr, unsigned int *mask);

#endif //_IP_CALC_H_
