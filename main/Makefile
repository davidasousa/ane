CC = gcc
CFLAGS = -g -I. -Wall -ggdb3 -std=c17 -Wshadow -Wvla -Werror -pedantic # -I. includes all files in the current directory

all: test ## Make The Target File To Be Ran

ane.o: ane.c ane.h
	$(CC) ${CFLAGS} ane.c -c

test.o: test.c
	$(CC) ${CFLAGS} test.c -c

test: ane.o test.o ## Building & Linking The Test Executable Together
	$(CC) ${CFLAGS} test.o ane.o -o test -lm && ./test

clean: 
	rm -f test.o ane.o test

PHONY: clean
