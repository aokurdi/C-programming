# getch
getCh () for linux

#NAME:

     getCh, getChNoEcho

#SYNOPSIS:

     #include <ti.h>
     int getCh();
     int getChNoEch();

#DESCRIPTION:

     getCh() reads the next character from the stdin and returns it as an int, doesn't wait for Enter to be pressed, it switch the terminal to a non-canonical
     mode and switch off buffering, it also handles keys F1 - F12, HOME, DEL, ARROWS, PgUP, and PgDN by reading terminfo database and returns a code starting
     from 256 for those keys souser can use those special keys in his project.
     getChNoEcho() disables echoing to the screen to handle things like password fields, it returns ascii value of the key or -1 in case of special keys.

#NOTES:
     getCh returns values greater than 255 in case of special keys like F1 - F12, so it shouldn't be used whith char cause a char is 8 bits
     and those values will cause overflow, instead use it with int:

     int ch = getCh ();
     printf ("%c", ch);

 included is test prog that shows how to use these functions and how they perform.

 #INSTALATION:
      in case you want to instll it as a system wide lib type:

      %: make lib

      %: make install

      then when compiling a project using this lib you should link it with: -lgetch

      in case you want to test before install
      %: make

      it will produce a file named getch_test, run it and take a look at the source.

 #LIMITATIONS:	

      	This is an example of how to use terminfo to get utilize an escape sequence for special keys in a c program... documentation is not yet ready but i ll be
		updating this file to fully explain for begeiners like me how read terminof db and use it in c programs to utilize using special keys in terminal apps 			without the need of using ncurses and/or complicated lib for such small projects,,,, i think it is also a good learning resource.
      	i would apriciate feedbacks and requests.

TODO:
    document all and explain all about dealing with terminfo in simple words.

