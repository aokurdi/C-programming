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


#include	<termios.h>
#include	<stdbool.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"getch.h"

/*####  Global  #################################################################### */
struct termios newAttrs, orgAttrs;

#define KSTR 9
#define TBLSIZE 53   /* No collosions` */

typedef struct key_ {
	char kstr[KSTR];
	char kname[KSTR];
	int  kcode;
	struct key_  *next;
} key;

/* Methods to create keys table */
bool insert_key_in_tbl (key *keyptr);
static unsigned int get_hash_value (const char *keystr);
static unsigned int search_keys_tbl (const char *keystr);

/* Key table and key strings */
key *KEYSTBL[TBLSIZE];

/* The order is important */
char *KEYS[] = {
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
	"\e[A",     /* UP    */
	"\e[B",     /* DN    */
	"\e[C",     /* FWD   */
	"\e[D",     /* BWD   */
	"\e[F",     /* END   */
	"\e[H",     /* HOME  */
};

/* The order is important */
char *KNAMES[] = {
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
 *  === Restore Attributes before exit =================================================
 * Restore original attributes at exit
 */
void
restore_org_attrs ( void ) {
	tcsetattr ( STDIN_FILENO, TCSANOW, &orgAttrs );
}

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
getCh ( ) {
	char ch[6];
	int code = 0;

	for (int i = 0; i < 6; ++i)
		ch[i] = '\0';

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
		fprintf (stderr, "Not a terminal.\n");
		exit (EXIT_FAILURE);
	}

	/* For some reason read blocks any previous printf */
	/* Flushing the stdout force to show it */
	fflush (stdout);

	/* Save the original terminal attrs */
	tcgetattr (STDIN_FILENO, &orgAttrs);
	atexit (restore_org_attrs);           /* Restore tty atexit */

	/* Convert non-canon mode */
	tcgetattr (STDIN_FILENO, &newAttrs);
	newAttrs.c_lflag &= ~(ICANON|ECHO);   /* Clear ICANON and ECHO. */
	newAttrs.c_cc[VMIN] = 1;              /* i will not show F(x) and ESC */
	newAttrs.c_cc[VTIME] = 1;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &newAttrs);

	/*
	 * Used a buffer to be able to catch those F(x) keys
	 * they all start with ESC char
	 */
	if (read (STDIN_FILENO, ch, 5)) {
		if (ch[0] == ESC) {       /* Handle ESC, F(x), and arrow keys */
			create_keys_tbl ( );
			code = search_keys_tbl (ch);
		} else {
			code = ch[0];
			putchar (ch[0]);       /* Print all other chars */
			fflush (stdout);
		}
		free_keys_tbl ( );
	}
	return code;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  getChNoEcho
 *  Description: get a char from user NO echo NOR wait for Enter key to be pressed
 *  			 NON-CANONICAL MODE
 *  Returns: ascii decimal value of char or -1 for special function keys
 * =====================================================================================
 */
int
getChNoEcho ( ) {
	char ch[10];

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
		fprintf (stderr, "Not a terminal.\n");
		exit (EXIT_FAILURE);
	}

	/* For some reason read blocks any previous printf */
	/* Flushing the stdout force to show it */
	fflush (stdout);

	/* Save the original terminal attrs */
	tcgetattr (STDIN_FILENO, &orgAttrs);
	atexit (restore_org_attrs);  /* Restore tty atexit */

	/* Convert non-canon mode */
	tcgetattr ( STDIN_FILENO, &newAttrs );
	newAttrs.c_lflag &= ~(ICANON|ECHO ); /* Clear ICANON and ECHO. */
	newAttrs.c_cc[VMIN] = 1;
	newAttrs.c_cc[VTIME] = 1;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &newAttrs);

	/* Used a buffer to be able to catch those F(x) keys
	 * they all start with ESC char */
	if (read (STDIN_FILENO, ch, 9)) {
	if (ch[0] == ESC)        /* Ignore ESC and F1 - F12 */
		ch[0] = -1;
	}
	return ch[0];
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
create_keys_tbl ( ) {
	key *tmp = NULL;
	register int i;

	/* Initialize the keys table */
	for (i = 0; i < TBLSIZE; i++) KEYSTBL[i] = NULL;

	/* Allocate mem for a key and generate code */
	for (i = 0; i < (sizeof (KEYS)/sizeof (KEYS[0])); i++) {
		tmp = malloc (sizeof (key));
		strcpy (tmp->kstr, KEYS[i]);    /* Key string */
		strcpy (tmp->kname, KNAMES[i]); /* Key string */
		tmp->kcode = i + 265;           /* Generate code for key */
		insert_key_in_tbl (tmp);        /* Insert key in keys table */
	}
}		/* -----  end of function create_keys_tbl  ----- */


/*
 * ===  Prints the keys Table  =========================================================
 *         Name:  print_keys_tbl
 *  Description: I am using this function to tweek the hash table for the keys
 * =====================================================================================
 */
void
print_keys_tbl ( ) {
	register int i;
	for (i = 0; i < TBLSIZE; ++i) {

		if (KEYSTBL[i] != NULL) {
			printf ("\t%d", i);
			key *tmp = KEYSTBL[i];

			while (tmp != NULL) {
				printf ("\t{key: %s\tkey string: ^[", tmp->kname);
				fwrite (&tmp->kstr[1], 1, 6, stdout); /* To skip printing esc char */
				printf ("\tcode: %d}", tmp->kcode);
				tmp = tmp->next;
			}
			printf ("\n");
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
insert_key_in_tbl (key *keyptr) {
	int index;
	if (keyptr == NULL)
		return false;

	index = get_hash_value (keyptr->kstr);
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
get_hash_value (const char *keystr) {
	unsigned int hashVal = 0;

	/* Creating hash index */
	while (*keystr != '\0') {
		hashVal = ((hashVal << 4) + (int)(*keystr));
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
search_keys_tbl (const char *keystr) {
	key *tmp;
	int index = get_hash_value (keystr);
	tmp = KEYSTBL[index];

	while (tmp != NULL && strcmp (tmp->kstr, keystr) !=0)
		tmp = tmp->next;

	return (tmp ? tmp->kcode : 27);
}		/* -----  end of function search_keys_tbl  ----- */


/*
 * ===  free the memory allocated for the keys table  ==================================
 *         Name:  free_keys_tbl
 *  Description:
 * =====================================================================================
 */
void
free_keys_tbl ( ) {
	key *tmp;
	for ( int i = 0; i < TBLSIZE; ++i ) {
		while (KEYSTBL[i] != NULL) {
			tmp = KEYSTBL[i];
			KEYSTBL[i] = KEYSTBL[i]->next;
			free (tmp);
		}
	}
	return ;
}		/* -----  end of function free_keys_tbl  ----- */
