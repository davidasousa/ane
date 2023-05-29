test: main.c calcStack.c parseString.c
	gcc main.c -lm -ggdb3 -o calc

run: calc
	valgrind ./calc input_file
