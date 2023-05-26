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

    int stack_size = execute(&stack, call_size); // stack size will end at the final array pos
    for(int idx = 0; idx <= stack_size; idx++)
        printf("%lg\n", stack.stack[idx]);
 
    free(stack.call);
    fclose(fp);
     
    return EXIT_SUCCESS;
}

// Questions: 
//
// 1. Function Pointers In A Heap - Why? And What Is The Ordering / Functionality Of The Heap 
//
// 2. What Should Be In Main, How To Divide C Files/ Number Of Lines In One C File
//
// 3. Read String -> Place Contents In Call Array (doubles) -> Execute On Stack (doubles), Is This Ordering Correct
