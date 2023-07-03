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

static void
run_I(double* stack, int* sp, heap_struct* heap, int* stack_size) {

    int heap_pos = get_op(pop(stack, sp));

    for(double* curr = &heap -> arr[heap_pos]; *curr != DELIMITER; curr++)
        parse_double(stack, *curr, sp, heap);

}

static void
run_bi(double* stack, int* sp, heap_struct* heap, int* stack_size) {

    int heap_pos_arr[] = {get_op(pop(stack, sp)), get_op(pop(stack,sp))};

    double results[100]; 
    int results_idx = 0;

    int og_sp = *sp;
    int leftest = INT_MAX;

    double* copy = malloc(sizeof(*copy) * (*stack_size));
    memcpy(copy, stack, sizeof(*copy) * (*stack_size));

    for(int i = 1; i >= 0; i--) {

        int first_push_pos = INT_MAX;

        for(double* curr = &heap -> arr[heap_pos_arr[i]]; *curr != DELIMITER; curr++) {
            parse_double(stack, *curr, sp, heap);
            if(first_push_pos > *sp)
                first_push_pos = *sp;
        }

        if(leftest > first_push_pos)
            leftest = first_push_pos;

        int parse_len = *sp - first_push_pos + 1;

        memcpy(&results[results_idx], &stack[first_push_pos], sizeof(*stack) * parse_len);
        results_idx += parse_len;

        *sp = og_sp;
        memcpy(stack, copy, sizeof(*stack) * (*stack_size));
    }

    *sp += leftest + results_idx - 1;
    memcpy(&stack[leftest], results, sizeof(*stack) * results_idx);

    free(copy);
    return;

}

void
run_comb(double* stack, int* sp, heap_struct* heap, int* stack_size, int arg) {
    void (*combs[2]) (double* stack, int* sp, heap_struct* heap, int* stack_size);
    combs[0] = run_I; combs[1] = run_bi;
    combs[arg] (stack, &(*sp), heap, &(*stack_size));
    return;
}


int 
execStack(double** stack, double* call, int call_size, int* sp, int* stack_size, heap_struct* heap) {
    int error_code = 0;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {

        if(*stack_size - 1 <= *sp) {
            *stack_size += 1000;
            *stack = realloc(*stack, sizeof(**stack) * (*stack_size));
        }

        if(get_tag(call[call_idx]) == COMBINATOR)
           run_comb(*stack, sp, heap, stack_size, get_op(call[call_idx]));
        else
            parse_double(*stack, call[call_idx], sp, heap);

    }

    return error_code;
}

