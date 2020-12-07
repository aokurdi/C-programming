#
# MAKEFILE
#

CC     = gcc
BIN    = getch_test
OBJ    = get_ch_driver.o
SRC    = get_ch_driver.c
LIB    = getch
SLIB   = libgetch
CFLAGS = -c -Wall -Werror -O2
LFLAGS = -fPIC
LINK   = -lgetch
LPATH  = /usr/local/lib
INCPATH = /usr/local/include


all: $(OBJ) $(LIB).o
	$(CC) $(OBJ) $(LIB).o -o $(BIN)

$(OBJ): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC)

lib: $(SLIB).so

$(LIB).o: $(LIB).c $(LIB).h
	$(CC) $(CFLAGS) $(LFLAGS) $(LIB).c

$(SLIB).so: $(LIB).o
	$(CC) -shared $(LIB).o -o $(SLIB).so

install:
	sudo cp $(SLIB).so $(LPATH)
	sudo cp $(LIB).h $(INCPATH)
	sudo ldconfig

clean:
	rm -rf ./*.o ./*~
	rm -rf ./a.out ./*.so ./getch_test
	rm -rf ./$(OBJ)
