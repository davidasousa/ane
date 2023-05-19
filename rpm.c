#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct stacknode {
    struct stacknode* prev; 
    float val;
} stacknode;

stacknode* push(stacknode* base, stacknode* top, char* elem)
{
    float val = 0;

    int idx;
    for(idx = 0; *(elem + idx) >= '0' && *(elem + idx) <= '9'; idx++)
    {
        val *= 10;
        val += *(elem + idx) - '0'; 
    }
    if(*(elem + idx++) == '.')
    {
        int digit = 1;
        while(*(elem + idx) >= '0' && *(elem + idx) <= '9')
            val += (*(elem + idx++) - '0') * pow(10, -1 * digit++); 
    }

    stacknode* new_node = malloc(sizeof(*new_node));
    new_node -> prev = NULL;

    if(val)
    {
        new_node -> val = val;
        if(base != NULL)
            new_node -> prev = top;
        return new_node;
    }
    
    return new_node;
}

stacknode* pop(stacknode* top, char* operation)
{
    float x, y;

    stacknode* curr = top;
    stacknode* temp = NULL;

    x = curr -> val;

    temp = curr;
    curr = curr -> prev;
    free(temp);

    y = curr -> val;
    
    temp = curr;
    curr = curr -> prev;
    free(temp);
    
    float ret_val;

    switch(*operation)
    {
        case '+':
            ret_val = x + y;
            break;

        case '-':
            ret_val = x - y;
            break;

        case 'x':
            ret_val = x * y;
            break;
            
        case '/':
            ret_val = x / y;
            break;

        default:
            break;
    }
    
    stacknode* new_node = malloc(sizeof(*new_node));
    *new_node = (stacknode) {.prev = curr, .val = ret_val};
    return new_node;
}

void print_stack(stacknode* top)
{
    if(top -> prev == NULL)
    {
        printf("%f ", top -> val);
        return;
    }
    else
    {
        print_stack(top -> prev);
        printf("%f ", top -> val);
    }
    return;
}

void free_stack(stacknode* top)
{
    if(top -> prev == NULL)
    {
        free(top);
        return;
    }
    else
    {
        free_stack(top -> prev);
        free(top);
    }
    return;
}

int main(int argc, char* argv[]) 
{
    stacknode* base = NULL;
    stacknode* top = NULL;
    for(int arg_idx = 1; arg_idx < argc; arg_idx++)
    {
        if(argv[arg_idx] == NULL)
        {
            fprintf(stdout, "ERROR: No Given Input\n");
            // free stack
            return EXIT_FAILURE;
        }

        if(base == NULL)
        {
            base = push(NULL, NULL, argv[arg_idx]);
            top = base;
        }
        else
        {
            int start = *(argv[arg_idx]) - 48;
            
            if(start >= 0 && start <= 9)
                top = push(base, top, argv[arg_idx]);
            else
                top = pop(top, argv[arg_idx]);

        }
    }
    
    printf("\n");
    print_stack(top);
    printf("\n\n");
    free_stack(top);

    return EXIT_SUCCESS;
}
