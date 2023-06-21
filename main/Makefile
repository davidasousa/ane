CC = gcc 
CFLAGS = -g -I. -Wall -ggdb3 -Wshadow -Wvla -Werror -pedantic # -I. includes all files in the current directory

all: ane.o nanbox.o test.o parseInput.o execStack.o prebuilt.o test ## Make The Target File To Be Ran

ane.o: ane.c ane.h
	$(CC) ${CFLAGS} ane.c -c

nanbox.o: nanbox.c
	$(CC) ${CFLAGS} nanbox.c -c

parseInput.o: parseInput.c
	$(CC) ${CFLAGS} parseInput.c -c

execStack.o: execStack.c
	$(CC) ${CFLAGS} execStack.c -c

prebuilt.o: prebuilt.c
	$(CC) ${CFLAGS} prebuilt.c -c

test.o: test.c
	$(CC) ${CFLAGS} test.c -c

test: ane.o test.o nanbox.o parseInput.o execStack.o prebuilt.o ## Building & Linking The Test Executable Together
	$(CC) ${CFLAGS} test.o ane.o nanbox.o parseInput.o execStack.o prebuilt.o -o test -lm

check: test
	./test

valcheck: test
	valgrind --track-origins=yes --leak-check=full -s ./test

clean: 
	rm -f test.o ane.o test nanbox.o parseInput.o execStack.o

PHONY: clean valcheck check
