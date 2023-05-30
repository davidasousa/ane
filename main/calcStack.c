#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

#ifndef CALCSTACK
#define CALCSTACK

// Ints Are Typically 32 Bits In Length

union d64 // Remove Typdef
{
    double box;
    uint64_t ubox;
} d64;

// Only Have One Operation
// Enum With The Type Of Operation
// The Number Of The Box Will Contain The Instructions

enum tag { OPERATION = 0x7ff8, PDF, UDF } tag; // One tag for built ins another tag for udfs
                                               // Predef & UDF
enum op_type {PLUS, MINUS, DIVIDE, MULTIPLY};

enum pdf_type {DUP, SWAP, ZAP};

typedef struct _stack {
    double stack[1000];
    double* call;
} stack;

double makeBox(int32_t operation, int32_t tag){ return (union d64){.ubox = operation | (int64_t)(tag) << 48}.box; }

uint32_t get_operator(double nanbox) { return (uint32_t)(union d64){.box = nanbox}.ubox; }

uint32_t get_tag(double nanbox) { return (union d64){.box = nanbox}.ubox >> 48 ;}

void push(double* stack, int* sp, double entry) {
    stack[++(*sp)] = entry;
    return;
}

double pop(double* stack, int* sp) { return stack[(*sp)--]; }

int is_num(double x){return x == x;}

static double add(double a, double b) {return b + a;}
static double sub(double a, double b) {return b - a;}
static double divide(double a, double b) {return b / a;}
static double mult(double a, double b) {return b * a;}

static void duplicate(double* stack, int* sp) {
    double copy = stack[*sp];
    (*sp)++;
    stack[*sp] = copy;
    return;
}

static void zap(double* stack, int* sp) {
    (*sp)--;
    return;
}

/*
static void swap(double* stack, int* sp) { //uncomment swap and see what happens
    double copy = stack[*sp];
    stack[*sp] = stack[*(sp - 1)];
    stack[*(sp - 1)] = copy;
    return;
}
*/


// The arithmatic methods should take the stack and pop from within those function
// dup swap zap
// Functions take the stack pointer

int execute(stack* stack, int call_size) {
    int sp = -1;
    double (*operation[4]) (double a, double b);
    operation[0] = add; operation[1] = sub; operation[2] = divide; operation[3] = mult; 

    void (*stack_func[1]) (double* a, int* b);
    stack_func[0] = duplicate; /*stack_func[1] = swap;*/ stack_func[2] = zap; // stack func 0

    for(int call_idx = 0; call_idx < call_size; call_idx++) {
        if(is_num(stack -> call[call_idx]))
            push(stack -> stack, &sp, stack -> call[call_idx]);
        else if(get_tag(stack -> call[call_idx]) == PDF){
            stack_func[get_operator(stack -> call[call_idx])] (stack -> stack, &sp);
        } else {
            double a = pop(stack -> stack, &sp);
            double b = pop(stack -> stack, &sp);
            double val = operation[get_operator(stack -> call[call_idx])] (a,b);            
            push(stack -> stack, &sp, val);
        } 
    }
    return sp;
}

#endif
