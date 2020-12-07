########################################
# Make file
########################################
TARGET = ipCalc
LIBS = -lpanel -lcurses
CC = gcc
CFLAGS = -Wall -Werror -c
DEBUG = -g
CFILES = mmenu.c ip_calc.c getIP.c
OBJECTS = mmenu.o ip_calc.o getIP.o
HEADERS = ip_calc.h mmenu.h getIP.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(OBJECTS): $(CFILES) $(HEADERS)
	$(CC) $(CFLAGS) $(CFILES)

clean:
	-rm -f *.o *~ core
	-rm -f $(TARGET)
