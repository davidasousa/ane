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
parse_double(double* stack, double arg, int* sp, heap_struct* heap) {

    if(get_tag(arg) == MATH_OPERATION) {

        run_math(stack, &(*sp), arg); // watch how/when error code is being returned

    } 
    else if(get_tag(arg) == LIST_OPERATION) {

        run_list_op(stack, &(*sp), heap, arg);

    }
    else if(get_tag(arg) == PREBUILT) {

        run_prebuilt(stack, &(*sp), arg);

    }
    else if(get_tag(arg) == STRING_OPERATION) {

        run_string_op(stack, &(*sp), heap -> arr, &heap -> hp, arg);

    }
    else if(get_tag(arg) == USERDEF) {
        int heap_pos = get_op(arg);

        for(double* quote = &heap -> arr[heap_pos]; *quote != DELIMITER; quote++)
            parse_double(stack, *quote, sp, heap);

    }
    else {

        push(stack, &(*sp), arg);

    };
    return 0;
}

static void
capture_list(double* stack, int* sp, heap_struct* heap, int og_hp, int* stack_size) { // copy everything larger than minsp

    double* cstack = malloc(sizeof(*cstack) * (*stack_size));
    memcpy(cstack, stack, sizeof(*cstack) * (*stack_size));

    int spcpy = *sp;
    int minsp = *sp + 1;

    for(double* curr = &heap -> arr[og_hp + 1]; *curr != DELIMITER; curr++) {

        if(get_tag(*curr) == LIST)
            capture_list(cstack, &spcpy, heap, get_op(*curr), stack_size); 
        else if(get_tag(*curr) == COMBINATOR)
            run_comb(cstack, &spcpy, heap, stack_size, get_op(*curr));
        else
            parse_double(cstack, *curr, &spcpy, heap);
        
        if(spcpy < minsp)
            minsp = spcpy;

    }

    int len = spcpy - minsp + 1;

    while(*sp != minsp - 1)
        pop(stack, sp);

    push(stack, sp, makeBox(heap -> hp, LIST));
    heap -> arr[heap -> hp++] = len;

    memcpy(&heap -> arr[heap -> hp], &cstack[minsp], sizeof(*cstack) * len);
    heap -> hp += len;

    heap -> arr[(heap -> hp)++] = DELIMITER;
    free(cstack);
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

        if(get_tag(call[call_idx]) == LIST)
            capture_list(*stack, sp, heap, get_op(call[call_idx]), stack_size);
        else if(get_tag(call[call_idx]) == COMBINATOR)
            run_comb(*stack, sp, heap, stack_size, get_op(call[call_idx]));
        else
            parse_double(*stack, call[call_idx], sp, heap);

    }

    return error_code;
}

