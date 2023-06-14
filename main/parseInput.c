#include "ane.h"

// Functions To Build Arrays For The Operations - Prebuilts, Math, Strings

static const char* prebuilt_ops[] = {
    [DUP] = "dup",
    [SWAP] = "swap",
    [ZAP] = "zap"
};

static const char* math_ops[] = {
    [PLUS] = "+",
    [MINUS] = "-",
    [MULTIPLY] = "*",
    [DIVIDE] = "/"
};

static const char* string_ops[] = {
    [STRCAT] = "strcat",
    [STRLEN] = "strlen"
};

static double
process_func(const char* input) { // Take A Given Char* Input & Associate With The Right Function

    int prebuilt_length = sizeof(prebuilt_ops) / sizeof(prebuilt_ops[0]);
    int math_length = sizeof(math_ops) / sizeof(math_ops[0]);
    int string_op_length = sizeof(string_ops) / sizeof(string_ops[0]);

    // Detecting Potentital Prebuilt
    for(int idx = 0; idx < prebuilt_length; idx++) {
        if(strcmp(input, prebuilt_ops[idx]) == 0) 
            return makeBox(idx, PREBUILT);
    }

    // Detecting Potentital Math Op
    for(int idx = 0; idx < math_length; idx++) {
        if(strcmp(input, math_ops[idx]) == 0) 
            return makeBox(idx, MATH_OPERATION);
    }

    // Detecting Potentital String Op
    for(int idx = 0; idx < string_op_length; idx++) {
        if(strcmp(input, string_ops[idx]) == 0) 
            return makeBox(idx, STRING_OPERATION);
    }
    return USERDEF;
}

static udf*
makeudf(const char** input, double* heap, int* hp, udf* functions[], int udf_count) {
    char* name = malloc(sizeof(*name) * 100);
    int pos = *hp;
    int idx = 0;

    *input += 2;

    while(**input != ' ') {
        name[idx++] = **input;
        *input += 1;
    }
    name[idx] = '\0';
    name = realloc(name, sizeof(name) * idx);
    int name_len = idx;
   
    idx = 0;

    while(**input != ';') {

        if(**input == ' ') {
            *input += 1;
            continue;
        }

        char arg[100];
        int char_elapsed;
        sscanf(*input, "%s%n", arg, &char_elapsed);

        if(**input >= '0' && **input <= '9') {
            double val;
            sscanf(*input, "%lg", &val);
            heap[*hp] = val;
            *hp += 1;
            idx++;
        } else {
            double nanbox = process_func(arg);

            if(nanbox == USERDEF) {
                for(int udf_idx = 0; udf_idx < udf_count; udf_idx++)
                    if(strcmp(arg, functions[udf_idx] -> name) == 0)
                        for(int func_idx = 0; func_idx < functions[udf_idx] -> args; func_idx++) {
                            heap[*hp] = heap[functions[udf_idx] -> hp + func_idx];
                            idx++;
                            *hp += 1;
                        }

            } else {
                heap[*hp] = nanbox;
                *hp += 1;
                idx++;
            }

        }
        *input += char_elapsed;
    }

    udf* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (udf) {.args = idx, .name = name, .name_strlen = name_len, .hp = pos};
 
    return new_udf;
}

static void
add_string(double* heap, int* hp, const char** arg) {

    *arg += 1; // Continuing The Flow Along The String

    char* curr;
    int idx = 0;

    for(const char* ch = *arg; *ch != '\"'; ch++) {

        curr = (char*) &heap[*hp];
        curr[idx++] = *ch;

        if(idx == 8) {
            (*hp)++;
            idx = 0;
        }
        *arg += 1;
    }
    curr = (char*) &heap[*hp];
    curr[idx] = '\0';
    (*hp)++;

    *arg += 1;
    return;
}

void
parseInput(double* call, const char* input, int* call_size, double* heap, int* hp, udf* functions[], int* udf_count) {
    int call_pointer = 0; // Total Number Of Elements In The Call Array
    const char* sp = input; // String Pointer

    double val; // Place Values Used For Parsing 
    int char_elapsed;
    char arg[100];

    while(*sp != '\0') {
        char_elapsed = 0;

        if(*sp == ' ') {

            sp++;

        } else if(*sp == ':') {

            functions[*udf_count] = makeudf(&sp, heap, hp, functions, *udf_count);
            *udf_count += 1;

        } else if(*sp == '"') {

            call[call_pointer++] = makeBox(*hp, STRING);
            add_string(heap, hp, &sp);

        } else if(*sp >= '0' && *sp <= '9') {

            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer++] = val;

        } else { // The thing in the call array is not recognizable by a prebuilt 

            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;

            double parse_result = process_func(arg);
            
            if(parse_result != USERDEF) // Not A UDF                             
                call[call_pointer++] = parse_result;
            else {
                for(int idx = 0; idx < *udf_count; idx++) {
                    if(strcmp(arg, functions[idx] -> name) == 0) {
                        for(int num = 0; num < functions[idx] -> args; num++) {
                            call[call_pointer++] = heap[functions[idx] -> hp + num];
                        }
                    }
                }
            }
            
        }
    }
    *call_size = call_pointer;
    return; 
}
