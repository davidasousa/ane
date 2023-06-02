#include "ane.h"

double
process_func(const char* func) { // Will take both prebuilts and udfs
    make_math_ops(); 
    make_prebuilt_ops();

    for(int idx = 0; idx < 3; idx++) {
       if(strcmp(func, prebuilt_ops[idx]) == 0) 
           return makeBox(idx, PREBUILT);
    }
    for(int idx = 0; idx < 4; idx++) {
       if(strcmp(func, math_ops[idx]) == 0) 
           return makeBox(idx, OPERATION);
    }
    return -1;
}
// FIX ARRAY ASSIGNMENT FOR THE HELPER FUNCTION

double* 
parseInput(const char* input, int* call_size) {
    double* call = malloc(sizeof(*call) * 1000);
    
    int call_pointer = 0; // Total Number Of Elements In The Call Array
    const char* sp = input; // String Pointer

    double val; // Place Values Used For Parsing 
    int char_elapsed;
    char arg[100];
    while(*sp != '\0') {
        char_elapsed = 0;

        if(*sp == ' ')
            sp++;
        else if(*sp >= '0' && *sp <= '9') {
            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer++] = val;
        }
        else {
            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;

            call[call_pointer++] = process_func(arg);
            }
        }
        *call_size = call_pointer;
        return call; 
}
