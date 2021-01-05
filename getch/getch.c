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
#include	<stdbool.h>
#include	<signal.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<stdio.h>


#include	"getch.h"

/*####  Defines #################################################################### */

#define KSTR	15
#define TBLSIZE 37


/*####  Global  #################################################################### */

typedef struct key key;
static struct termios orgTerm;
static key *KEYSTBL[TBLSIZE];

struct key {
	char kstr[KSTR];
	char kname[KSTR];
	int  kcode;
	struct key *next;
};

/* The order is important */
static char *KEYS[] = {
	"\eOP",     /* F1 */
	"\eOQ",
	"\eOR",
	"\eOS",
	"\e[15~",
	"\e[17~",
	"\e[18~",
	"\e[19~",
	"\e[20~",
	"\e[21~",
	"\e[23~",
	"\e[24~",   /* F12   */
	"\e[2~",    /* INSRT */
	"\e[3~",    /* DEL   */
	"\e[5~",    /* PgUp  */
	"\e[6~",    /* PgDn  */
	"\eOA",     /* UP    */
	"\eOB",     /* DN    */
	"\eOC",     /* FWD   */
	"\eOD",     /* BWD   */
	"\eOF",     /* END   */
	"\eOH",     /* HOME  */
};


/*####  Private routines  ########################################################## */

static unsigned int search_keys_tbl ( const char * );
static unsigned int get_hash_value ( const char * );
int  get_key_code ( const char * );
bool insert_key_in_tbl ( key * );
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

	/* Leave keyboard transmit mode */
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
	rawTerm.c_iflag &= ~( IXON | ICRNL );  /* Input flags */

	/* ISIG: disable Ctl-c Ctl-z, ICANON: disable stdin buffering
	 * IEXTEN: disable Ctl-v, ECHO: disable echo */
	rawTerm.c_lflag &= ~( ICANON | ECHO | IEXTEN | ISIG );  /* Local falgs */

	/* OPOST: disable output Processing */
	rawTerm.c_oflag &= ~( OPOST );  /* Output flags */

	/* Force char size to be 8 bits */
	rawTerm.c_cflag |= ( CS8 );

	rawTerm.c_cc[VMIN]  = 1; /* Read at least 1 char */
	rawTerm.c_cc[VTIME] = 0; /* Don't wait */

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

	create_keys_tbl( );
	code = search_keys_tbl( kstr );
	free_keys_tbl( );
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

/* ###  The following functions are used internally to build a table to handle
 * FUNCTION keys
 */
/*
 * ===  Creates keys table  ============================================================
 *         Name:  create_keys_tbl
 *  Description:
 * =====================================================================================
 */
	void
create_keys_tbl ( )
{
	key *tmp = NULL;
	register int i;

	/* Initialize the keys table */
	for( i = 0; i < TBLSIZE; i++ ) KEYSTBL[i] = NULL;

	/* Allocate mem for a key and generate code */
	for( i = 0; i < (sizeof (KEYS)/sizeof (KEYS[0])); i++ ) {
		tmp = malloc( sizeof (key) );
		strcpy( tmp->kstr, KEYS[i] );    /* Key string */
		strcpy( tmp->kname, KNAMES[i] ); /* Key string */
		tmp->kcode = i + 256;           /* Generate code for key */
		insert_key_in_tbl( tmp );        /* Insert key in keys table */
	}
}		/* -----  end of function create_keys_tbl  ----- */


/*
 * ===  Prints the keys Table  =========================================================
 *         Name:  print_keys_tbl
 *  Description: I am using this function to tweek the hash table for the keys
 * =====================================================================================
 */
	void
print_keys_tbl ( )
{
	register int i;
	for( i = 0; i < TBLSIZE; ++i ) {

		if( KEYSTBL[i] != NULL ) {
			printf( "\t%d", i );
			key *tmp = KEYSTBL[i];

			while( tmp != NULL ) {
				printf( "\t{key: %s\tkey string: ^[%s", tmp->kname, &tmp->kstr[1] );
				printf( "\tcode: %d}", tmp->kcode );
				tmp = tmp->next;
			}
			printf( "\n" );
		}
	}
}		/* -----  end of function print_keys_tbl  ----- */


/*
 * ===  Inerer keys into the hash table  ===============================================
 *         Name:  insert_key_in_tbl
 *  Description:
 * =====================================================================================
 */
	bool
insert_key_in_tbl (key *keyptr)
{
	int index;
	if( keyptr == NULL )
		return false;

	index = get_hash_value( keyptr->kstr );
	keyptr->next = KEYSTBL[index];
	KEYSTBL[index] = keyptr;
	return true;
}		/* -----  end of function insert_key_in_tbl  ----- */


/*
 * ===  Get a hash index dependig on key string  =======================================
 *         Name:  get_hash_value
 *  Description:
 * =====================================================================================
 */
	static unsigned int
get_hash_value ( const char *keystr )
{
	long int hashVal = 0;

	/* Creating hash index */
	while( *keystr != '\0' && *keystr != 0x7E ) {
		hashVal = ((hashVal << 8) | *keystr) + 33;
		keystr++;
	}

	/* Treming index to table size */
	return hashVal % TBLSIZE;
}		/* -----  end of function get_hash_value  ----- */


/*
 * ===  Search the tabel using hash index  =============================================
 *         Name:  search_keys_tbl
 *  Description:
 * =====================================================================================
 */
	static unsigned int
search_keys_tbl( const char *keystr )
{
	key *tmp;
	int index = get_hash_value( keystr );
	tmp = KEYSTBL[index];

	while( tmp != NULL && strcmp (tmp->kstr, keystr) !=0 )
		tmp = tmp->next;

	return( tmp ? tmp->kcode : 27 );
}		/* -----  end of function search_keys_tbl  ----- */


/*
 * ===  free the memory allocated for the keys table  ==================================
 *         Name:  free_keys_tbl
 *  Description:
 * =====================================================================================
 */
	void
free_keys_tbl ( )
{
	key *tmp;
	for( int i = 0; i < TBLSIZE; ++i ) {
		while( KEYSTBL[i] != NULL ) {
			tmp = KEYSTBL[i];
			KEYSTBL[i] = KEYSTBL[i]->next;
			free( tmp );
		}
	}
	return ;
}		/* -----  end of function free_keys_tbl  ----- */
