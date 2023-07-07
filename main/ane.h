#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

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
heap_struct {
    double* arr;
    int hp;
    int heap_size;
} heap_struct;

typedef struct
udf {
    char* name;
    int hp;
    int name_strlen;
    int args;
} udf;

typedef struct 
udf_struct {
    udf** functions;
    int udf_count;
    int udf_lim;
} udf_struct;

typedef struct 
stack_struct {
    double* stack;
    int sp;
    int stack_size;
} stack_struct;

double 
makeBox(uint32_t op, uint32_t tag);

int 
parse_double(double* stack, double arg, int* sp, heap_struct* heap);

// ENUMS

enum tag_type { MATH_OPERATION = 0x7ff8, PREBUILT, USERDEF, STRING, STRING_OPERATION, READWRITE, DELIMITER, QUOTATION, COMBINATOR, LIST, LIST_OPERATION}; // nantag 
enum prebuilt_op { DUP, SWAP, ZAP, CLEAR};
enum math_op { PLUS, MINUS, MULTIPLY, DIVIDE };
enum string_op { STRCAT, STRLEN };
enum comb_op { I , BI };
enum list_op { LIST_LENGTH, LIST_PUSH_IDX };

// PREBUILT OPERATIONS

void
push(double* stack, int* sp, double entry);

double
pop(double* stack, int* sp);

int
run_math(double* stack, int* sp, double arg);

void 
duplicate(double* stack, int* sp);

void
swap(double* stack, int* sp);

void 
zap(double* stack, int* sp);

void
run_prebuilt(double* stack, int* sp, double arg);

void
string_strcat(double* stack, int* sp, double* heap, int* hp);

void
run_string_op(double* stack, int* sp, double* heap, int* hp, double arg);

void
run_comb(double* stack, int* sp, heap_struct* heap, int* stack_size, int arg);

void
run_list_op(double* stack, int* sp, heap_struct* heap, double arg);

// Nanbox Operations

int 
is_num(double value);

double
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

uint32_t
get_tag(double nanbox);

// String Reading 
int 
parseInput(double* call, const char* input, heap_struct* heap, udf_struct* udfs);

int // Returns error code
execStack(double** stack_ptr, double* call, int call_size, int* sp, int* stack_size, heap_struct* heap);

void
ane(FILE* input, int* valid_pass, FILE* output);
