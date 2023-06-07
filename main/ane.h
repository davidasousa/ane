#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

union 
d64 {
    double nbox; // nbox is the nanbox itself - cannot be operated on must be casted
    uint64_t ubox; // ubox is the casted form meant for operations on bits
};

typedef struct 
callStack {
    double* call;
    double stack[1000];
} callStack;

typedef struct
UDF {
    double* function;
    char* name;
    int args;
} UDF;

double 
makeBox(uint32_t op, uint32_t tag);

enum tag_type { MATH_OPERATION = 0x7ff8, PREBUILT, USERDEF, STRING, STRING_OPERATION }; // nantag 
enum error {ERROR = -1};

enum prebuilt_type { DUP, SWAP, ZAP};
enum math_op_type { PLUS, MINUS, MULTIPLY, DIVIDE };
enum string_op_type { STRCAT };

double
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

uint32_t
get_tag(double nanbox);

void read_string(char* strings, int* strings_pos, const char** sp);

double* 
parseInput(const char* input, int* call_size, UDF** udfs, int* udf_count, double* heap, int* heap_size, char strings[], int* string_pos);

void // Returns the size of the stack at finish
calcStack(callStack* stack, int call_size, int* sp, FILE* output, char* strings, int* string_pos);

double* 
ane(FILE* input, int* valid_pass, FILE* output);
// The Ane function returns the stack at completion

