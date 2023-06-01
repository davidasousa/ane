#pragma once

#ifndef ANE
#define ANE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

struct arrays;

union 
d64;

double 
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

extern enum tag_type { OPERATION = 0x7ff8, PREBUILT, USERDEF } tag_type; // nantag 
extern enum operation_type { PLUS, MINUS, MULTIPY, DIVIDE } operation_type;
extern enum prebuilt_type { DUP, SWAP, ZAP} prebuilt_type;

double
makeBox(uint32_t op, uint32_t tag);

double* 
parseInput(const char* input, int* call_size);
// Parse Input Takes The Input Argument As A Whole String Than Returns The Call Array Based On The Input 
// Call Size Is The Total Size Of The Array

double* 
ane(const char* input, int* valid_pass);
// The Ane function returns the stack at completion

#endif

