#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    "getch.h"

/* Mapping CTRL-KEY */
#define CTRL_KEY(k) ((k) & 0x1f)
#define CLEAR_SCR "\e[2J\e[1H"
    int
main (int argc, char **argv)
{
    int ch;

    /* Test key code returns */
    printf( CLEAR_SCR );   /* clear screan */
    printf( "\n\t\tCTRL-Q: go next test, CTRL-P: print keys table\n" );
    printf( "\t\tHit a key to get key code: " );

    while( (ch = getCh( )) != CTRL_KEY('q') ) {
        if( ch == CTRL_KEY('p') ) {
            printf( "\n\n" );
            create_keys_tbl ( );
            print_keys_tbl  ( );
            free_keys_tbl   ( );
        } else if( ch > 255 ) {
            printf( "\n\t\tKey is: %-10s\tkey code is: %i\n", KNAMES[ch - 256], ch );
        } else {
            printf( "\n\t\tKey is: <%c>\tkey code is: %i\n",
                    ( isprint(ch) ? ch : ' '), ch );
        }

        printf( "\n\t\tHit a key to continue...." );
        getCh( );
        printf( CLEAR_SCR );   /* clear screan */
        printf( "\n\t\tCTRL-Q: go next test, CTRL-P: print keys table\n" );
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
}
