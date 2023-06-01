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

struct arrays;

extern union 
d64 {
    double nbox; // nbox is the nanbox itself - cannot be operated on must be casted
    uint64_t ubox; // ubox is the casted form meant for operations on bits
} d64;

double 
makeBox(uint32_t op, uint32_t tag);

extern enum tag_type { OPERATION = 0x7ff8, PREBUILT, USERDEF } tag_type; // nantag 
extern enum operation_type { PLUS, MINUS, MULTIPY, DIVIDE } operation_type;
extern enum prebuilt_type { DUP, SWAP, ZAP} prebuilt_type;

double
makeBox(uint32_t op, uint32_t tag);

uint32_t
get_op(double nanbox);

uint32_t
get_tag(double nanbox);

double* 
parseInput(const char* input, int* call_size);

double* 
ane(const char* input, int* valid_pass);
// The Ane function returns the stack at completion

#endif

