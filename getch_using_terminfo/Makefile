# MAKEFILE
CC     = gcc
BIN    = test
LIB    = getch.o ti.o
LIBSRC = ti.c getch.c
HEADER = ti.h
SLIB   = libti.so
CFLAGS = -c -Wall -Werror -O2
LFLAGS = -fPIC
LINK   = -lti
LPATH  = /usr/local/lib
INCPATH = /usr/local/include


all: $(LIB) $(BIN).c
	$(CC) $(BIN).c $(LIB) -o $(BIN)

lib: $(SLIB)

$(LIB): $(LIBSRC) $(HEADER)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBSRC)

$(SLIB): $(LIB)
	$(CC) -shared -o $(SLIB) $(LIB)

install:
	sudo cp $(SLIB) $(LPATH)
	sudo cp ti.h $(INCPATH)
	sudo ldconfig

clean:
	rm -rf ./*.o ./*~
	rm -rf ./a.out ./*.so ./test
