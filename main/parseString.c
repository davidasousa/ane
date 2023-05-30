#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "calcStack.c"

#ifndef PARSESTRING
#define PARSESTRING

double* read_input(char* input, int* call_size) {
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
        else { // Better Way Of Doing This? Note: Write Things Down
            switch(*sp) {
                case ' ':
                    break;
                case '+':
                    call[call_pointer++] = makeBox(PLUS, OPERATION);
                    break;
                case '-':
                    call[call_pointer++] = makeBox(MINUS, OPERATION);
                    break;
                case '*':
                    call[call_pointer++] = makeBox(MULTIPLY, OPERATION);
                    break;
                case '/':
                    call[call_pointer++] = makeBox(DIVIDE, OPERATION);
                    break;
                default:
                    char arg[10];
                    int chars;
                    sscanf(sp, "%s%n", arg, &chars);
                    sp += chars;

                    if(strcmp(arg, "dup") == 0) // Better Way Of Doing This 
                        call[call_pointer++] = makeBox(DUP, PDF);
                    if(strcmp(arg, "swap") == 0)
                        call[call_pointer++] = makeBox(SWAP, PDF);
                    if(strcmp(arg, "zap") == 0)
                        call[call_pointer++] = makeBox(ZAP, PDF);
            }
            sp++; 
        }
    }

    *call_size = call_pointer;

    if(call_pointer < 2) {
        free(call);
        return NULL; // Error Checking
    }


    return call;
}

#endif
