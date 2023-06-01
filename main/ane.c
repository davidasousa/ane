#include "ane.h"

typedef struct 
_arrays {
    double* call;
    double stack[1000];
} arrays;

union 
d64 {
    double nbox; // nbox is the nanbox itself - cannot be operated on must be casted
    uint64_t ubox; // ubox is the casted form meant for operations on bits
} d64;

double* 
ane(const char* input, int* valid_pass) {

    return NULL;
}
