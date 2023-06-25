#include "ane.h"

void 
push(double* stack, int* sp, double entry) {
    stack[++(*sp)] = entry;
    return;
}

double
pop(double* stack, int* sp) { return stack[(*sp)--]; }

// Premade Math Operations

static int
parse_double(double* stack, double arg, int* sp, heap_struct* heap) {

        if(get_tag(arg) == MATH_OPERATION) {

            run_math(stack, &(*sp), arg); // watch how/when error code is being returned

        } 
        else if(get_tag(arg) == PREBUILT) {

            run_prebuilt(stack, &(*sp), arg);

        }
        else if(get_tag(arg) == STRING) {

            push(stack, &(*sp), arg);

        }
        else if(get_tag(arg) == STRING_OPERATION) {
 
            run_string_op(stack, &(*sp), heap -> arr, &heap -> hp, arg);
            
        }
        else if(get_tag(arg) == USERDEF) {
            int heap_pos = get_op(arg);

            for(double* quote = &heap -> arr[heap_pos]; *quote != DELIMITER; quote++)
                parse_double(stack, *quote, sp, heap);

        }
        else if(is_num(arg) || (get_tag(arg) == STRING)) {

            push(stack, &(*sp), arg);
            
        } else 
            return 0;
        return 1;
}

static void
run_I(double* stack, double* call, int* sp, heap_struct* heap, int comb_pos) {

    int quote_pos;
    for(int idx = 0; idx < comb_pos; idx++) {
        if(get_tag(call[idx]) == QUOTATION)
            quote_pos = idx;
    }

    int heap_pos = get_op(call[quote_pos]);

    for(double* quote = &heap -> arr[heap_pos]; *quote != DELIMITER; quote++) {
        printf("1");
        parse_double(stack, *quote, sp, heap);

    }
    printf("\n");
    

}

int 
execStack(double* stack, double* call, int call_size, int* sp, heap_struct* heap) {
    int error_code = 0;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {

        if(get_tag(call[call_idx]) == COMBINATOR) {
           run_I(stack, call, sp, heap, call_idx);

            continue;
        }

        parse_double(stack, call[call_idx], sp, heap);

    }

    return error_code;
}

