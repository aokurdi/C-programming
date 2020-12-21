# getch 
getCh () for linux

#NAME:

     getCh, getChNoEcho

#SYNOPSIS:

     #include <getch.h>
     int getCh();
     int getChNoEch();
     
#DESCRIPTION:

     getCh() reads the next character from the stdin and returns it as an int, doesn't wait for Enter to be pressed, it switch the terminal to a non-canonical
     mode and switch off buffering, it also handles keys F1 - F12, HOME, DEL, ARROWS, PgUP, and PgDN returns a code starting from 265 to 287 for those keys so
     user can use those special keys in his project.
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
 
      This lib is assuming you are usig a terminal emulator in VT100 mode, like most terminals you'll find on a typical linux destro;
      but if the terminal is using another mode such as VT220 then it will send diffirent escape sequence, to make this lib portable 
      and compatible with all terminals you will need to use the terminfo database which is out of the scope of this example because 
      it will involve using term.h and cureses.h header files and you will request the key code from the data base.
      I hope this will help someone who is trying to learn c programming.

