#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

// Ints Are Typically 32 Bits In Length

union d64 // Remove Typdef
{
    double box;
    uint64_t ubox;
} d64;

// Only Have One Operation
// Enum With The Type Of Operation
// The Number Of The Box Will Contain The Instructions

enum tag { OPERATION = 0x7ff8 } tag;

enum type {
    PLUS,
    MINUS,
    DIVIDE,
    MULTIPLY
};

typedef struct _stack 
{
    double stack[1000];
    double* call;
} stack;

double makeBox(int32_t operation){ return (union d64){.ubox = operation | (int64_t)(OPERATION) << 48}.box; }

uint32_t get_operator(double nanbox) { return (uint32_t)(union d64){.box = nanbox}.ubox; }

void push(double* stack, int* sp, double entry)
{
    stack[++(*sp)] = entry;
    return;
}

double pop(double* stack, int* sp) { return stack[(*sp)--]; }

double* read_input(FILE* fp, int* call_size)
{
    double* call = malloc(sizeof(*call) * 1000);
    
    int call_pointer = 0;
    for(char ch = fgetc(fp); !feof(fp); ch = fgetc(fp))
    {
        if(ch != ' ' && (ch > '9' || ch < '0'))
        {
            switch(ch)
            {
            case '-':
                call[call_pointer++] = makeBox(MINUS);
                break;
            case '+':
                call[call_pointer++] = makeBox(PLUS);
                break;
            case '*':
                call[call_pointer++] = makeBox(MULTIPLY);
                break;
            case '/':
                call[call_pointer++] = makeBox(DIVIDE);
                break;
            }
            ch = fgetc(fp);
        }
        else
        {
            double val = 0;

            // Reading The Double Value
            while(ch >= '0' && ch <= '9')
            {
                val *= 10;
                val += ch - '0';
                ch = fgetc(fp);
            }
            if(ch == '.')
            {
                int decimal_place = 1;
                ch = fgetc(fp);

                while(ch >= '0' && ch <= '9')
                {
                    val += pow(10, -1 * decimal_place++) * (ch - '0');
                    ch = fgetc(fp);
                }
            }           
            call[call_pointer++] = val; 
        }
    }
    *call_size = call_pointer;
    return call;
}

int is_num(double x){return x == x;}

double add(double a, double b) {return b + a;}
double sub(double a, double b) {return b - a;}
double divide(double a, double b) {return b / a;}
double mult(double a, double b) {return b * a;}

int execute(stack* stack, int call_size)
{
    int sp = -1;
    double (*func[4]) (double a, double b);
    func[0] = add; 
    func[1] = sub;
    func[2] = divide;
    func[3] = mult; 
    
    for(int call_idx = 0; call_idx < call_size; call_idx++)
    {
        if(is_num(stack -> call[call_idx]))
            push(stack -> stack, &sp, stack -> call[call_idx]);
        else
        {
            double a = pop(stack -> stack, &sp);
            double b = pop(stack -> stack, &sp);
            double val = func[get_operator(stack -> call[call_idx])] (a,b);            
            push(stack -> stack, &sp, val);
        } 
    }
    return sp;
}

