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
udf {
    char* name;
    int hp;
    int name_strlen;
    int args;
} udf;

double 
makeBox(uint32_t op, uint32_t tag);

enum tag_type { MATH_OPERATION = 0x7ff8, PREBUILT, USERDEF, STRING, STRING_OPERATION, READWRITE, DELIMITER}; // nantag 
enum prebuilt_op { DUP, SWAP, ZAP};
enum math_op { PLUS, MINUS, MULTIPLY, DIVIDE };
enum string_op { STRCAT, STRLEN };

// Nanbox Operations
int 
is_num(double value);

double
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

uint32_t
get_tag(double nanbox);
//

// String Reading 
void
parseInput(double* call, const char* input, int* call_size, double* heap, int* hp, udf* functions[], int* udf_count);

int // Returns error code
execStack(double* stack, double* call, int call_size, int* sp, double* heap, int* hp);

void
ane(FILE* input, int* valid_pass, FILE* output);
