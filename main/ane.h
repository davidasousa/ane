#pragma once

#ifndef ANE
#define ANE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

struct stack;

extern union 
d64 {
    double nbox; // nbox is the nanbox itself - cannot be operated on must be casted
    uint64_t ubox; // ubox is the casted form meant for operations on bits
} d64;

typedef struct 
callStack {
    double* call;
    double stack[1000];
} callStack;

double 
makeBox(uint32_t op, uint32_t tag);

extern enum tag_type { OPERATION = 0x7ff8, PREBUILT, USERDEF } tag_type; // nantag 
extern enum error {ERROR = -1} error;

extern enum prebuilt_type { DUP = 0, SWAP, ZAP} prebuilt_type;
extern const char* prebuilt_ops[];
void make_prebuilt_ops();

extern enum math_op_type { PLUS = 0, MINUS, MULTIPLY, DIVIDE } math_op_type;
extern const char* math_ops[]; // Array Relates The Strings To The Enums
void make_math_ops();

double
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

uint32_t
get_tag(double nanbox);

double* 
parseInput(const char* input, int* call_size);

void // Returns the size of the stack at finish
calcStack(callStack* stack, int call_size, int* sp);

double* 
ane(FILE* input, int* valid_pass);
// The Ane function returns the stack at completion

#endif

