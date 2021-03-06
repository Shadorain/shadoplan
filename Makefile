# Shadoplan Makefile
CC=gcc
CFLAGS=-I. -lncurses -g
DEPS=config.h
OBJ=shadoplan.o options.o shadomenu.o
BDIR=/usr/local

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

sp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

install: 
	mkdir -p $(BDIR)/bin
	cp -f sp $(BDIR)/bin
	chmod 755 $(BDIR)/bin/sp

clean:
	rm -f *.o sp

uninstall: 
	rm -f $(BDIR)/bin/sp

.PHONY: clean install uninstall
