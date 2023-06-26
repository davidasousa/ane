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
        else if(get_tag(arg) == QUOTATION) {

            push(stack, &(*sp), arg);
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
        else if(is_num(arg)) {

            push(stack, &(*sp), arg);
            
        } else 
            return 0;
        return 1;
}

static int 
find_quote(double* stack, int size) {
    int pos = 0;
    for(int idx = 0; idx <= size; idx++)
        if(get_tag(stack[idx]) == QUOTATION)
            pos = idx;
    return pos;
}

static void
remove_from_stack(double* stack, int* size, int pos) {
    for (int i = pos; i < (*size - 1); i++)
            stack[i] = stack[i + 1];
    (*size)--;
    return;
}

static void
run_I(double* stack, double* call, int* sp, heap_struct* heap, int* stack_size) {

    int quote_pos = find_quote(stack, *sp);
    int heap_pos = get_op(stack[quote_pos]);
    remove_from_stack(stack, stack_size, quote_pos);
    (*sp)--;

    for(double* curr = &heap -> arr[heap_pos]; *curr != DELIMITER; curr++) {
        parse_double(stack, *curr, sp, heap);
    }
}


int 
execStack(double** stack, double* call, int call_size, int* sp, int* stack_size, heap_struct* heap) {
    int error_code = 0;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {

        if(*stack_size - 1 <= *sp) {
            *stack_size += 1000;
            *stack = realloc(*stack, sizeof(**stack) * (*stack_size));
        }

        if(get_tag(call[call_idx]) == COMBINATOR) {
           run_I(*stack, call, &(*sp), heap, stack_size);


            continue;
        }

        parse_double(*stack, call[call_idx], sp, heap);

    }

    return error_code;
}

