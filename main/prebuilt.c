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

static int
run_I(double* stack, int* sp, heap_struct* heap, int* stack_size) {

    int heap_pos = get_op(pop(stack, sp));
    for(double* curr = &heap -> arr[heap_pos]; *curr != DELIMITER; curr++) {

        if(get_tag(*curr) == QUOTATION) {
            parse_double(stack, *curr, sp, heap); // push the nanbox from the heap but skip the actual contents
            while(*curr != DELIMITER)
                curr++;
        }
        else
            parse_double(stack, *curr, sp, heap);
    }
    return INT_MAX;
}

static int
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

        if(first_push_pos == INT_MAX) // skipping empty quotes
            continue;

        memcpy(&results[results_idx], &stack[first_push_pos], sizeof(*stack) * parse_len);
        results_idx += parse_len;

        *sp = og_sp;
        memcpy(stack, copy, sizeof(*stack) * (*stack_size));
    }

    if(results_idx > 0) {
        *sp += leftest + results_idx - 1;
        memcpy(&stack[leftest], results, sizeof(*stack) * results_idx);
    }

    free(copy);
    return leftest;

}

int
run_comb(double* stack, int* sp, heap_struct* heap, int* stack_size, int arg) {
    int (*combs[2]) (double* stack, int* sp, heap_struct* heap, int* stack_size);
    combs[0] = run_I; combs[1] = run_bi;
    int min = combs[arg] (stack, &(*sp), heap, &(*stack_size));
    return min;
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
//
//next ----- list in list

static void
push_list_idx(double* stack, int* sp, heap_struct* heap) {
    int idx = pop(stack, sp);
    int list_hp = get_op(pop(stack, sp));

    double* val = &heap -> arr[list_hp + 1];

    for(int i = 0; i < idx; i++) { // skiping over the quote instructions present after the nanbox for the quote in the heap - reference good notes picture
        if(get_tag(*val) == QUOTATION || get_tag(*val) == LIST) {
            while(*val != DELIMITER) {
                val++;
            }
        }
        val++;
    }

    push(stack, sp, *val);
    return;
}

static void
list_cat(double* stack, int* sp, heap_struct* heap) {
    assert(get_tag(stack[*sp]) == LIST);
    assert(get_tag(stack[*sp - 1]) == LIST);

    int cdr_hp = get_op(pop(stack, sp));
    int con_hp = get_op(pop(stack, sp));

    push(stack, sp, makeBox(heap -> hp, LIST));

    heap -> arr[(heap -> hp)++] = heap -> arr[con_hp] + heap -> arr[cdr_hp];
    
    memcpy(&heap -> arr[heap -> hp], &heap -> arr[con_hp + 1], sizeof(double) * (heap -> arr[con_hp]));

    (heap -> hp) += heap -> arr[con_hp];

    memcpy(&heap -> arr[heap -> hp], &heap -> arr[cdr_hp + 1], sizeof(double) * (heap -> arr[cdr_hp]));

    (heap -> hp) += heap -> arr[cdr_hp];

    heap -> arr[(heap -> hp++)] = DELIMITER;

    return;
}

static void
list_sum(double* stack, int* sp, heap_struct* heap) {
    int list_hp = get_op(pop(stack, sp));
    int sum = 0;

    for(double* curr = &heap -> arr[list_hp + 1]; *curr != DELIMITER; curr++)
        sum += *curr;

    push(stack, sp, (double)sum);
    return;
}

static void
list_prod(double* stack, int* sp, heap_struct* heap) {
    int list_hp = get_op(pop(stack, sp));
    int sum = 1;

    for(double* curr = &heap -> arr[list_hp + 1]; *curr != DELIMITER; curr++)
        sum *= *curr;

    push(stack, sp, (double)sum);
    return;
}

static void
cleave(double* stack, int* sp, heap_struct* heap) {
    int list_hp = get_op(pop(stack, sp));
    double* list_head = &heap -> arr[list_hp];

    double* stack_cpy = malloc(sizeof(*stack_cpy) * 1000);
    memcpy(stack_cpy, stack, sizeof(*stack_cpy) * 1000);

    double results[1000];
    int ridx = 0;

    int og_sp = *sp;
    int leftest = INT_MAX;

    for(double* quote = list_head + 1; *quote != DELIMITER; quote++) {
        int quote_hp = get_op(*quote);

        int fpp = INT_MAX;

        for(double* curr = &heap -> arr[quote_hp]; *curr != DELIMITER; curr++) {
            parse_double(stack, *curr, sp, heap);
            if(fpp > *sp)
                fpp = *sp;
        }

        if(leftest > fpp)
            leftest = fpp;

        int parse_len = *sp - fpp + 1;

        if(fpp == INT_MAX)
            continue;

        memcpy(&results[ridx], &stack[fpp], sizeof(*stack) * parse_len);
        ridx += parse_len;

        *sp = og_sp;
        memcpy(stack, stack_cpy, sizeof(*stack) * 1000);
    }

    if(ridx > 0) { // leave on stack
        /* leave on stack else non comment is leave as list
        *sp += leftest + ridx - 1;
        memcpy(&stack[leftest], results, sizeof(*stack) * ridx);
        */ 

        int new_list_head = (heap -> hp)++;
        heap -> arr[new_list_head] = leftest + ridx - 1;
        memcpy(&heap -> arr[heap -> hp], results, sizeof(*stack) * ridx);
        heap -> hp += leftest + ridx - 1;

        while(*sp != leftest - 1)
            pop(stack, sp);

        push(stack, sp, makeBox(new_list_head, LIST));
    }

    free(stack_cpy);

    return;
}

static void
map(double* stack, int* sp, heap_struct* heap) {
    int quote_hp = get_op(pop(stack, sp));
    int list_hp = get_op(pop(stack, sp));

    int list_count = heap -> arr[list_hp];

    int new_list_hp = (heap -> hp)++;
    push(stack, sp, makeBox(new_list_hp, LIST));
    
    for(int idx = 0; idx < list_count; idx++) {
        double temp[1000];
        int tp = 0;
        memcpy(temp, &heap -> arr[list_hp + idx + 1], sizeof(double));

        for(double* curr = &heap -> arr[quote_hp]; *curr != DELIMITER; curr++)
            parse_double(temp, *curr, &tp, heap);

        memcpy(&heap -> arr[(heap -> hp++)], &temp[0], sizeof(double));
    }
    heap -> arr[(heap -> hp)++] = DELIMITER;

    return;
}

static void
ifte(double* stack, int* sp, heap_struct* heap) {
    double con = pop(stack, sp);
    double cdr = pop(stack, sp);
    double cond = pop(stack, sp);

    if(cond) {
        for(double* curr = &heap -> arr[get_op(cdr)]; *curr != DELIMITER; curr++) 
            parse_double(stack, *curr, sp, heap); 
    } else {
        for(double* curr = &heap -> arr[get_op(con)]; *curr != DELIMITER; curr++) 
            parse_double(stack, *curr, sp, heap);  
    }

    return;
}

static void
spread(double* stack, int* sp, heap_struct* heap) {
    int list_pos = get_op(pop(stack, sp));
    int qcount = heap -> arr[list_pos];
    
    double* entries = malloc(sizeof(*entries) * qcount);
    for(int idx = 0; idx < qcount; ++idx) {
        entries[qcount - idx - 1] = pop(stack, sp);
    }

    double* results = malloc(sizeof(double) * 1000);
    int ridx = 0;

    for(int idx = 0; idx < qcount; ++idx) {
        results[ridx] = entries[idx];
        int quote_idx = get_op(heap -> arr[list_pos + 1 + idx]);

        for(double* curr = &heap -> arr[quote_idx]; *curr != DELIMITER; curr++) 
            parse_double(results, *curr, &ridx, heap);   
        ridx++;
    }

    memcpy(&stack[*sp + 1], results, sizeof(double) * ridx);
    *sp += ridx;
    
    free(results);
    free(entries);
    return;
}

void
run_list_op(double* stack, int* sp, heap_struct* heap, double arg) {
    void (*list_op[9]) (double* stack, int* sp, heap_struct* heap);
    list_op[0] = list_len; list_op[1] = push_list_idx; list_op[2] = list_cat; list_op[3] = list_sum; list_op[4] = list_prod; list_op[5] = cleave; list_op[6] = map; list_op[7] = ifte; list_op[8] = spread;
    list_op[get_op(arg)] (stack, &(*sp), heap); 
    return;
}

// things to talk about:
// expand the number of tags in nanbox was unable to do this
// recursive function intersection - best way to make lists and quotes nested within eachother and store them in the heap properly 
//
