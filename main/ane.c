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
ane(const char* input, int* valid_pass) {

    return NULL;
}
