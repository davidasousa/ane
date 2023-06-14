#include "ane.h"

static void 
push(double* stack, int* sp, double entry) {
    stack[++(*sp)] = entry;
    return;
}

static double
pop(double* stack, int* sp) { return stack[(*sp)--]; }

// Premade Math Operations
static double
add(double a, double b) {return a + b;}

static double
sub(double a, double b) {return b - a;}

static double
mult(double a, double b) {return b * a;}

static double
divide(double a, double b) {return b / a;}
//

static int
run_math(double* stack, int* sp, double arg) {
    double (*math_op[4]) (double a, double b);
    math_op[0] = add; math_op[1] = sub; math_op[2] = mult; math_op[3] = divide;

    double a = pop(stack, &(*sp));
    double b = pop(stack, &(*sp));

    push(stack, &(*sp), math_op[get_op(arg)] (a, b));
    return 0;
}

static void
duplicate(double* stack, int* sp) {
    double copy = stack[*sp];
    push(stack, &(*sp), copy); 

    return;
}

static void
swap(double* stack, int* sp) {
    double copy = stack[*sp];
    stack[*sp] = stack[(*sp - 1)];
    stack[(*sp - 1)] = copy;

    return;
}

static void
zap(double* stack, int* sp) { (*sp)--; }

// Prebuilt Functions
static void
run_prebuilt(double* stack, int* sp, double arg) {
    void (*pre_op[3]) (double* stack, int* sp);
    pre_op[0] = duplicate; pre_op[1] = swap; pre_op[2] = zap;
    pre_op[get_op(arg)] (&(*stack), &(*sp));

    return;
}

static void
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

static void
run_string_op(double* stack, int* sp, double* heap, int* hp, double arg) { // this code is shit
    void (*string_op[1]) (double* stack, int* sp, double* heap, int* hp);
    string_op[0] = string_strcat;
    string_op[get_op(arg)] (stack, &(*sp), heap, &(*hp));
    return;
}

int
execStack(double* stack, double* call, int call_size, int* sp, double* heap, int* hp) {
    int error_code = 0;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {
        if(is_num(call[call_idx]) || (get_tag(call[call_idx]) == STRING)) {

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
