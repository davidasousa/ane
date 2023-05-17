#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

enum type {NUMBER, FUNCTION, OPERATOR, QUOTATION, COMBINATOR};

typedef struct _element { // Are these bad naming conventions?
    
    enum type class;

    union content {
        double value;
        char* function;
        char* operator;
        char* quotation;
        char* combinator;
    } content;

} element;

typedef struct _stacknode {
    element element;
    struct _stacknode* next;
} stacknode;

// Input is a file containing one string

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL)
        return EXIT_FAILURE;

    for(char ch = fgetc(fp); !feof(fp); ch = fgetc(fp))
    {
        if(ch >= '0' && ch <= '9')
        {
            double val = ch - '0';
            ch = fgetc(fp);

            bool decimal_occurance = false;
            int sig_fig = 1;

            while((ch >= '0' && ch <= '9') || ch == '.') // Looping after the initial digit
            {
                if(ch == '.')
                {
                    decimal_occurance = true;
                    ch = fgetc(fp);
                }

                if(decimal_occurance)
                    val += (ch - '0') / pow(10, sig_fig++);
                else {
                    val *= 10;
                    val += ch - '0';
                }
                ch = fgetc(fp);     
            }

            printf("%lf\n", val);
        }
    }

    return EXIT_SUCCESS;
}
