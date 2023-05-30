test: testFile.c
	gcc testFile.c -lm -ggdb3 -o test

run:
	./test

valgrind:
	valgrind --track-origins=yes --leak-check=full ./test
