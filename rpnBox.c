#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

// Ints Are Typically 32 Bits In Length

typedef union d64 
{
    double box;
    int64_t ubox;
} d64;

enum tag
{
   NUMBER = 0x7ff8,
   //OPERATION
   PLUS,
   MINUS,
   MULTIPLY,
   DIVIDE
} tag;

d64 makeBox(int32_t val, int32_t tag) {return (d64){.ubox = val | (int64_t)tag << 48};}

int unbox_val(d64 nanbox) {return (int32_t)(nanbox.ubox);}

int get_tag(d64 nanbox) {return (int32_t)(nanbox.ubox >> 48);}

void push(double* stack, int* sp, double nanbox)
{
    stack[++(*sp)] = nanbox;
    return;
}

double pop(double* stack, int* sp)
{
    return stack[(*sp)--]; 
}

double get_val(double nanbox)
{
    d64 temp = (d64)nanbox;
    return unbox_val(temp);
}

int main(void)
{
    double stack[1000];
    int sp = -1;

    double call[1000] = {makeBox(12, NUMBER).box, makeBox(4, NUMBER).box, makeBox(0, MULTIPLY).box, makeBox(1, NUMBER).box, makeBox(0, PLUS).box, makeBox(31, NUMBER).box}; // Can be the union - d64 or doubles both would work 
                                                                                                                                                                              // Call is the instructions given in an array, the stack is the stack, sp points to the stack top

    for(int idx = 0; idx < 6; idx++)
    {
        int tag = get_tag((d64)call[idx]);

        switch(tag)
        {
            case PLUS:
                double a = get_val(pop(stack, &sp));
                double b = get_val(pop(stack, &sp));
                push(stack, &sp, makeBox(a + b, NUMBER).box);
                break;
            case MINUS:
                a = get_val(pop(stack, &sp));
                b = get_val(pop(stack, &sp));
                push(stack, &sp, makeBox(b - a, NUMBER).box);
                break;
            case MULTIPLY:
                a = get_val(pop(stack, &sp));
                b = get_val(pop(stack, &sp));
                push(stack, &sp, makeBox(a * b, NUMBER).box);
                break;
            case DIVIDE:
                a = get_val(pop(stack, &sp));
                b = get_val(pop(stack, &sp));
                push(stack, &sp, makeBox(b / a, NUMBER).box);
                break;
            default:
                push(stack, &sp, call[idx]);
                break;
        }
    }

    printf("%d %lf\n", sp, get_val(stack[sp]));
    printf("%d %lf", sp - 1, get_val(stack[sp - 1]));
    printf("\nSize Of The Nan Box Used %lu", sizeof(d64));


    return EXIT_SUCCESS;
}
