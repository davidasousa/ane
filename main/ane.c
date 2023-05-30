#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "calcStack.c"
#include "parseString.c"

double* ane(char* input, int* pass) {
    int call_size;
    int error_number = 0;
    stack stack;
    stack.call = read_input(input, &call_size);
    
    if(stack.call == NULL)
        error_number = 1;


    int sp = 0;
    if(stack.call != NULL)
        sp = execute(&stack, call_size); // stack size will end at the final array pos
    free(stack.call);

    //  if(sp != 0)
    //     error_number = 2;

    // ERROR SEGMENT
    if(error_number > 0) {
        *pass = EXIT_FAILURE;
        switch(error_number) {
            case 1: 
                fprintf(stdout, "Error: Not Enought Arguments Present To Compute");
                break; 
                /*
            case 2: 
                fprintf(stdout, "Error: Multiple Values In Stack At Finish");
                return stack.stack;
                break; 
                */
        }
        return 0;
    }
    
    double* ans = malloc(sizeof(*ans) * (sp + 1));
    memmove(ans, stack.stack, (sp + 1) * sizeof(*ans));

    *pass = EXIT_SUCCESS;
    return ans;
}
