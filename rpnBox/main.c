#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "rpnBox.c"

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL)
        return EXIT_FAILURE;

    int call_size;
    stack stack;
    stack.call = read_input(fp, &call_size);

    execute(stack, call_size);

    free(stack.call);
    fclose(fp);
     
    return EXIT_SUCCESS;
}
