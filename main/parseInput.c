#include "ane.h"

static const char* prebuilt_ops[] = {
    [DUP] = "dup",
    [SWAP] = "swap",
    [ZAP] = "zap"
};

static const char* math_ops[] = {
    [PLUS] = "+",
    [MINUS] = "-",
    [MULTIPLY] = "*",
    [DIVIDE] = "/"
};

static double
process_func(const char* func) { // Proccess all but udf
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

static UDF*
makeUDF(const char** input) {
    int char_elapsed;
    int arg_count = 0;
    char* name = malloc(sizeof(*name) * 100); // why does using stack'd not work

    sscanf(*input, ": %s %n", name, &char_elapsed);
    *input += char_elapsed;

    const char* copy = *input;

    while(*copy != ';') {
        if(*copy == ' ')
            arg_count++;
        copy++;
    }

    UDF* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (UDF) {.args = arg_count, .name = name};
    
    double* contents = malloc(sizeof(*contents) * arg_count);
    for(int cont_idx = 0; cont_idx < arg_count; cont_idx++) {
        char* args = malloc(sizeof(*args) * 100);
        sscanf(*input, "%s %n", args, &char_elapsed);
        *input += char_elapsed;

        if(*args >= '0' && *args <= '9'){
            double num = 0;
            sscanf(args, "%lg", &num);
            contents[cont_idx] = num;
        } else
            contents[cont_idx] = process_func(args);
        free(args);
    }

    new_udf -> function = contents;

    return new_udf;
}

// finish udfs
// do some error testing
// "start quotations ..?"

double* 
parseInput(const char* input, int* call_size, UDF* udfs[], int* udf_count) {
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
        else if(*sp == ':') {
            udfs[(*udf_count)++] = makeUDF(&sp); 
        } else if(*sp >= '0' && *sp <= '9') {
            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer++] = val;
        } else {
            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer] = process_func(arg);

            if(call[call_pointer] == -1) { // udf === -1
                for(int udf_idx = 0; udf_idx < *udf_count; udf_idx++) {
                    
                    if(strcmp(arg, udfs[udf_idx] -> name) == 0) {
                        double* function = udfs[udf_idx] -> function;
                        for(int func_idx = 0; func_idx < udfs[udf_idx] -> args; func_idx++) {
                            call[call_pointer++] = function[func_idx];
                        }
                    }
                }                                        
            } else
                call_pointer++;
            }
        }
        *call_size = call_pointer;
        return call; 
}
