parse: main.c
	gcc main.c -lm -o a.out

rpm: rpm.c
	gcc -ggdb3 rpm.c -lm -o rpm
