#include "ane.h"

static void 
push(double* stack, int* sp, double entry) {
    stack[++(*sp)] = entry;
    return;
}

static double
pop(double* stack, int* sp) { return stack[(*sp)--]; }

static int
is_num(double val) { return val == val;}

static double
add(double a, double b) {return a + b;}

static double
sub(double a, double b) {return b - a;}

static double
mult(double a, double b) {return b * a;}

static double
divide(double a, double b) {return b / a;}

static void
run_operation(callStack* stack, int* sp, double arg) {
    double (*math_op[4]) (double a, double b);
    math_op[0] = add; math_op[1] = sub; math_op[2] = mult; math_op[3] = divide;

    double a = pop(stack -> stack, &(*sp));
    double b = pop(stack -> stack, &(*sp));
    push(stack -> stack, &(*sp), math_op[get_op(arg)] (a, b));
    return;
}

int 
calcStack(callStack* stack, int call_size) {
    int sp = -1;

    for(int call_idx = 0; call_idx < call_size; call_idx++) {
        if(is_num(stack -> call[call_idx]))
            push(stack -> stack, &sp, stack -> call[call_idx]);
        else if(get_tag(stack -> call[call_idx]) == OPERATION) {
            run_operation(stack, &sp, stack -> call[call_idx]);
        }
    }
    
    return sp + 1;
}
