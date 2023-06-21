#include "ane.h"

void 
push(double* stack, int* sp, double entry) {
    stack[++(*sp)] = entry;
    return;
}

double
pop(double* stack, int* sp) { return stack[(*sp)--]; }

// Premade Math Operations

int
execStack(double* stack, double* call, int call_size, int* sp, double* heap, int* hp) {
    int error_code = 0;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {

        if(get_tag(call[call_idx]) == QUOTATION)
            continue;
        else if(is_num(call[call_idx]) || (get_tag(call[call_idx]) == STRING)) {

            push(stack, &(*sp), call[call_idx]);

        }
        else if(get_tag(call[call_idx]) == MATH_OPERATION) {

            run_math(stack, &(*sp), call[call_idx]); // watch how/when error code is being returned

        } 
        else if(get_tag(call[call_idx]) == PREBUILT) {

            run_prebuilt(stack, &(*sp), call[call_idx]);

        }
        else if(get_tag(call[call_idx]) == STRING) {

            push(stack, &(*sp), call[call_idx]);

        }
        else if(get_tag(call[call_idx]) == STRING_OPERATION) {
 
            run_string_op(stack, &(*sp), heap, &(*hp), call[call_idx]);
            
        }
    }

    return error_code;
}

void
I_comb(double* heap, int* call_size, double* call) {

    int pos = get_op(call[*call_size - 1]);
    for(double* curr = &heap[pos]; *curr != DELIMITER; curr++) 
        call[(*call_size)++] = *curr;
    return;
}

void
run_comb(double* heap, double* call, int* call_size, double arg) {

    void (*combs[1]) (double* heap, int* call_size, double* call);
    combs[0] = I_comb; 
    combs[get_op(arg)] (heap, call_size, call);

    return;
}
