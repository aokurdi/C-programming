#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    "getch.h"


    int
main (int argc, char **argv)
{
    int ch;

    /* Test key code returns */
    while (ch != 'q') {
        printf ("\e[1;1H\e[2J");  /* clear screan */
        printf ("\t\t\vPress a key: ");

        ch = getCh ( );
        printf ("\n\t\tKey is: %c, key code is: %i\n", (isprint (ch) ? ch : '-'), ch);
        printf ("\t\tpress 'q' to exit or 'p' to print keys table\n");
        ch = getChNoEcho ( );

        if (ch == 'p') {
            create_keys_tbl ( );
            print_keys_tbl ( );
            free_keys_tbl ( );
            getCh ( );
        }
    }


    /* Test response speed and Echo Mode */
    printf ("\v\t================  Testing Response  ===============\n");
    printf ("\t\tStart typing, press 'ESC' to end: \n");
    printf ("\t\t");

    do {
        ch = getCh ( );
        if (ch == 127)   /* Backspace */
            printf ("\b \b");
    } while ('\e' != ch);


    /* Test No Echo Mode */
    printf ("\v\t================  Testing NO ECHO  ================\n");
    printf ("\t\tType your password 'ENTER to end': ");
    char *buffer = malloc (sizeof (char));
    int size = 0;

    do {
        ch = getChNoEcho ( );
        if (ch != -1 && ch != 127) { /* Regular char */
            buffer[size] = ch;
            size++;
            buffer = realloc (buffer, (sizeof (char) * (size + 1)));
            buffer[size + 1] = '\0';

        } else if (ch == 127) {      /* Handle backspace */
            if (size > 0) {
                size --;
                buffer[size] = '\0';
            } else {
                size = 0;
                buffer[0] = '\0';
            }
        }
    } while (ch != '\n');
    printf ( "\n\t\tYour secret code is: %s\n", buffer );

    /* Clean up and Exit */
    if (buffer) free (buffer);
    return EXIT_SUCCESS;
}
