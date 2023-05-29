#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "calcStack.c"

#ifndef PARSESTRING
#define PARSESTRING

double* read_input(char* input, int* call_size)
{
    double* call = malloc(sizeof(*call) * 1000);
    int call_pointer = 0;
    
    char* sp = input;
    while(*sp != '\0') {
        if(*sp >= '0' && *sp <= '9') {
            double val;
            int chars;
            sscanf(sp, "%lg%n", &val, &chars);
            sp += chars;
            call[call_pointer++] = val;
        }
        else { 
            switch(*sp) {
                case '+':
                    call[call_pointer++] = makeBox(PLUS);
                    break;
                case '-':
                    call[call_pointer++] = makeBox(MINUS);
                    break;
                case '*':
                    call[call_pointer++] = makeBox(MULTIPLY);
                    break;
                case '/':
                    call[call_pointer++] = makeBox(DIVIDE);
                    break;
            }
            sp++; 
        }
    }

    *call_size = call_pointer;
    return call;
}

#endif
