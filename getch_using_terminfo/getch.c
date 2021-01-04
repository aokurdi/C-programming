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

#include    <ctype.h>
#include	<termios.h>
#include	<signal.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<stdio.h>

#include	"ti.h"

/*####  Defines #################################################################### */

#define KSTR	16


/*####  Global  #################################################################### */

static struct termios orgTerm;


/*####  Private routines  ########################################################## */

int  get_key_code ( const char * );
void handle_SIGTERM ( int );
int  init_term ( void );
int  read_key ( );


/*
 *  === Restore Attributes before exit =================================================
 * Restore original attributes at exit
 */
	void
restore_org_attrs ( void )
{
	tcsetattr( STDIN_FILENO, TCSAFLUSH, &orgTerm );

	/* Leave keyboard transmit modem; ncurses = "rmkx" */
	printf( "\e[?1l\e>" );
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  handle_SIGKILL
 *  Description: restores term and exit in case of kill is recieved
 * =====================================================================================
 */
	void
handle_SIGTERM ( int sig )
{
	restore_org_attrs();
	printf( "\e[2J\e[1H\n\n" );  /* Clear Screen */
	fprintf( stderr, "\tFatal: Recived SIGTERM... %d\n", sig );
	exit( 128 + sig );
}		/* -----  end of function handle_SIGKILL  ----- */

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
	int
getCh ( )
{
	/* Make sure stdin is a terminal. */
	if( !isatty (STDIN_FILENO) ) {
		fprintf( stderr, "Not a terminal.\n" );
		return -1;
	}

	/* Save the original terminal attrs */
	if( tcgetattr(STDIN_FILENO, &orgTerm) == -1 ) {
		perror( "tcgetattr" );
		return -1;
	}

	/* In case we recieve kill signal we want to restore term */
	signal( SIGTERM, handle_SIGTERM );

	/* Make sure keyboard transmit mode is enabled
	 * Thi is important to map some keys correctly like arrow
	 * down key and end key; ncurses "smkx" mode */
	printf( "\e[?1h\e=" );

	/* Flush out any buffered output */
	fflush( stdout );

	init_term( );
	int keyCode = read_key( );

	if( isprint(keyCode) )
		if( write( STDOUT_FILENO, &keyCode,  1 ) < 1 )
			perror( "write" );

	restore_org_attrs( );

	return keyCode;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  init_term
 *  Description: put Terminal in Raw mode
 * =====================================================================================
 */
	int
init_term ( void )
{
	struct termios rawTerm;
	tcgetattr( STDIN_FILENO, &rawTerm );

	/* IXON: disable Ctl-s Ctal-q
	 * ICRNL: disable \r\n combo so Ctl-m returns 13 */
	rawTerm.c_iflag &= ~( IXON | ICRNL | IGNBRK | PARMRK );  /* Input flags */
	rawTerm.c_iflag &= ~( BRKINT | INPCK | ISTRIP | INLCR);

	/* ISIG: disable Ctl-c Ctl-z, ICANON: disable stdin buffering
	 * IEXTEN: disable Ctl-v, ECHO: disable echo */
	rawTerm.c_lflag &= ~( ICANON | ECHO | IEXTEN | ISIG );  /* Local falgs */

	/* OPOST: disable output Processing */
	rawTerm.c_oflag &= ~( OPOST );  /* Output flags */

	/* Force char size to be 8 bits */
	rawTerm.c_cflag &= ~( CSIZE | PARENB );      /* Control mode */
	rawTerm.c_cflag |= ( CS8 );      /* Control mode */

	rawTerm.c_cc[VMIN]  = 1; /* Read at least 1 char */
	rawTerm.c_cc[VTIME] = 0; /* No timer */

	tcsetattr( STDIN_FILENO, TCSAFLUSH, &rawTerm );
    return 0;
}		/* -----  end of function init_term  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  read_key
 *  Description: get a key from user
 * =====================================================================================
 */
	int
read_key ( )
{
	int code, flag;
	char keyStr[KSTR];
	memset( keyStr, '\0', KSTR );

	flag = read( STDIN_FILENO, keyStr, KSTR );

	if( flag > 1 )       /* Handle F(x), and arrow keys */
		code = get_key_code( keyStr );
	else
		code = *keyStr;

	return code;
}		/* -----  end of function read_key  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_key_code
 *  Description: process the key and return a code
 * =====================================================================================
 */
	int
get_key_code ( const char *kstr )
{
	int code;

	code = get_code_from_ti( kstr );
	return code;
}		/* -----  end of function get_key_code  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getChNoEcho
 *  Description: get a char from user NO echo NOR wait for Enter key to be pressed
 *  			 NON-CANONICAL MODE
 *  Returns: ascii decimal value of char or key code for special function keys
 * =====================================================================================
 */
	int
getChNoEcho ( )
{
	/* Make sure stdin is a terminal. */
	if( !isatty (STDIN_FILENO) ) {
		fprintf( stderr, "Not a terminal.\n" );
		return -1;
	}

	/* Save the original terminal attrs */
	if( tcgetattr(STDIN_FILENO, &orgTerm) == -1 ) {
		perror( "tcgetattr" );
		return -1;
	}

	/* In case we recieve kill signal we want to restore term */
	signal( SIGTERM, handle_SIGTERM );

	fflush( stdout );
	init_term( );
	int keyCode = read_key( );

	restore_org_attrs( );

	return keyCode;
}
