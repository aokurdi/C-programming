#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    "getch.h"
#include    "ti.h"

/* Mapping CTRL-KEY */
#define CTRL_KEY(k) ((k) & 0x1f)
#define CLEAR_SCR "\e[2J\e[1H"

#include	<stdlib.h>

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description: Test getch lib
 * =====================================================================================
 */
    int
main (int argc, char **argv)
{
    int ch;

    /* Test key code returns */
    printf( CLEAR_SCR );   /* clear screan */
    printf( "\n\t\tCTRL-Q: go next test\n" );
    printf( "\t\tHit a key to get key code: " );

    while( (ch = getCh( )) != CTRL_KEY('q') ) {
        if( ch > 255 ) {
            printf( "\n\t\tKey name: <%s>%25s%i\n", get_key_name(ch), "Key code: ", ch );
        } else {
            printf( "\n\t\tKey name: <%c>%25s%i\n",
                    ( isprint(ch) ? ch : ' ' ), "Key code: ", ch );
        }

        printf( "\n\t\tHit a key to continue...." );
        getCh( );
        printf( CLEAR_SCR );   /* clear screan */
        printf( "\n\t\tCTRL-Q: go next test\n" );
        printf( "\t\tHit a key to get key code: " );

    }


    /* Test response speed and Echo Mode */
    printf( "\n\n\t\t================  Testing Response  ================\n\n" );
    printf( "\t\tStart typing, CTRL-Q to end: \n" );
    printf( "\t\t" );

    do {
        ch = getCh( );
        if( ch == 127 )   /* Backspace */
            printf( "\b \b" );
    } while( CTRL_KEY('q')!= ch );


    /* Test No Echo Mode */
    printf( "\n\n\t\t================  Testing NO ECHO  =================\n\n" );
    printf( "\t\tType your password 'CTRL-J to end': " );
    char *buffer = malloc( sizeof (char) );
    int size = 0;

    do {
        ch = getChNoEcho( );
        if( ch != -1 && ch != 127 ) {
            buffer[size] = ch;
            size++;
            buffer = realloc( buffer, (sizeof (char) * (size + 1)) );
            buffer[size + 1] = '\0';

        } else if( ch == 127 ) {
            if( size > 0 ) {
                size --;
                buffer[size] = '\0';
            } else {
                size = 0;
                buffer[0] = '\0';
            }
        }
    } while( ch != 10 );
    printf ( "\n\t\tYour secret code is: %s\n\n", buffer );

    if (buffer) free (buffer);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
