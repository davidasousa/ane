#include "ane.h"

const char* math_ops[4];
void make_math_ops() {
    math_ops[PLUS] = "+";
    math_ops[MINUS] = "-";
    math_ops[MULTIPLY] = "*";
    math_ops[DIVIDE] = "/";
}

const char* prebuilt_ops[3];
void make_prebuilt_ops() {
    prebuilt_ops[DUP] = "dup";
    prebuilt_ops[SWAP] = "swap";
    prebuilt_ops[ZAP] = "zap";
}

double* 
ane(FILE* stream, int* valid_pass) {
    char str[1000];
    callStack stack;
    int sp = -1;

    // This is where the udfs will be stored - makes sense?
    // udf function will take the sp - compile and store the udf in the structure present here in ane each udf is an array of doubles containing the resective steps of the udf
    //double* udfs[10]; // this will be a param passed into calcstack
    
    while(fgets(str, sizeof(str), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0';

        if(strcmp(str, "q") == 0)
            break;

        int call_size = 0;
        stack.call = parseInput(str, &call_size);
        calcStack(&stack, call_size, &sp);

        for(int idx = 0; idx <= sp; idx++)
            printf("%lg ", stack.stack[idx]);
        printf("\n");
        free(stack.call);
        // stack call is malloced and freed everytime- fix this FIX
    }
    printf("ANE Closed Successfully");

    return NULL;
}
