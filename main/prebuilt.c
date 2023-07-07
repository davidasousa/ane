#include "ane.h"

// MATH 

double
add(double a, double b) {return a + b;}

double
sub(double a, double b) {return b - a;}

double
mult(double a, double b) {return b * a;}

double
divide(double a, double b) {return b / a;}
//

int
run_math(double* stack, int* sp, double arg) {
    double (*math_op[4]) (double a, double b);
    math_op[0] = add; math_op[1] = sub; math_op[2] = mult; math_op[3] = divide;

    double a = pop(stack, &(*sp));
    double b = pop(stack, &(*sp));

    push(stack, &(*sp), math_op[get_op(arg)] (a, b));
    return 0;
}

// PREBUILT OPERATIONS

void
duplicate(double* stack, int* sp) {
    double copy = stack[*sp];
    push(stack, &(*sp), copy); 

    return;
}

void
swap(double* stack, int* sp) {
    double copy = stack[*sp];
    stack[*sp] = stack[(*sp - 1)];
    stack[(*sp - 1)] = copy;

    return;
}

void
zap(double* stack, int* sp) { (*sp)--; }

void
clear(double* stack, int* sp) { *sp = -1; }

// Prebuilt Functions
void
run_prebuilt(double* stack, int* sp, double arg) {
    void (*pre_op[4]) (double* stack, int* sp);
    pre_op[0] = duplicate; pre_op[1] = swap; pre_op[2] = zap; pre_op[3] = clear;
    pre_op[get_op(arg)] (&(*stack), &(*sp));

    return;
}

void
string_strcat(double* stack, int* sp, double* heap, int* hp) {
    int cdr_heap_pos = get_op(pop(stack, &(*sp)));
    int con_heap_pos = get_op(pop(stack, &(*sp)));

    char string[100];

    strcpy(string, (char*) &heap[con_heap_pos]);
    strcat(string, (char*) &heap[cdr_heap_pos]);

    push(stack, &(*sp), makeBox(*hp, STRING));

    char* curr;
    int idx = 0;
    for(char* ch = string; *ch != '\0'; ch++) {

        curr = (char*) &heap[*hp];
        curr[idx++] = *ch;

        if(idx == 8) {
            (*hp)++;
            idx = 0;
        }
    }
    curr = (char*) &heap[*hp];
    curr[idx] = '\0';
    (*hp)++;

    return;
}

void
run_string_op(double* stack, int* sp, double* heap, int* hp, double arg) { // this code is shit
    void (*string_op[1]) (double* stack, int* sp, double* heap, int* hp);
    string_op[0] = string_strcat;
    string_op[get_op(arg)] (stack, &(*sp), heap, &(*hp));
    return;
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

static void
list_len(double* stack, int* sp, heap_struct* heap) {
    assert(get_tag(stack[*sp]) == LIST); 
    double list_size = heap -> arr[(get_op(pop(stack, sp)))];
    push(stack, sp, list_size);
    return;
}

//static void
//map_reduce

static void
push_list_idx(double* stack, int* sp, heap_struct* heap) {
    int idx = pop(stack, sp);
    int list_hp = get_op(pop(stack, sp));
    push(stack, sp, heap -> arr[list_hp + 1 + idx]);
    return;
}

void
run_list_op(double* stack, int* sp, heap_struct* heap, double arg) {
    void (*list_op[2]) (double* stack, int* sp, heap_struct* heap);
    list_op[0] = list_len; list_op[1] = push_list_idx;
    list_op[get_op(arg)] (stack, &(*sp), heap);
    return;
}

// things to talk about:
// expand the number of tags in nanbox was unable to do this
// recursive function intersection - best way to make lists and quotes nested within eachother and store them in the heap properly 
//
