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

static const char* quote_ops[] = {
    [I] = "I",
};

static double
process_func(const char* input) { // Take A Given Char* Input & Associate With The Right Function

    int prebuilt_length = sizeof(prebuilt_ops) / sizeof(prebuilt_ops[0]);
    int math_length = sizeof(math_ops) / sizeof(math_ops[0]);
    int string_op_length = sizeof(string_ops) / sizeof(string_ops[0]);
    int quote_op_length = sizeof(quote_ops) / sizeof(quote_ops[0]);

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

    // Detecting Potentital Quote Op
    for(int idx = 0; idx < quote_op_length; idx++) {
        if(strcmp(input, quote_ops[idx]) == 0)
            return makeBox(idx, COMBINATOR);
    }
    return USERDEF; // Userdef because it wasnt detected by another type, this should check if it is in the valid array of udfs and then return error else 
}


static int 
find_len(const char* str, char delimiter) {
    int idx = 0;
    while(*(str + idx) != delimiter) 
        idx++;
    return idx;
}

static void
add_string(double* heap, int* hp, int* heap_size, const char** arg, int strlen) {
    memcpy(&heap[*hp], *arg, sizeof(char) * (strlen - 1));
    ((char*)&heap[*hp])[strlen] = '\0';
    *hp += (strlen / 8 + 1); 
    return;
}

static bool
is_char_num(char ch) { return (ch >= '0' && ch <= '9'); }

static void
write_udf(double* heap, int* hp, udf* functions[], int udf_size, char* arg) {

    for(int idx = 0; idx < udf_size; idx++)

        if(strcmp(arg, functions[idx] -> name) == 0) { 

            for(int func_idx = 0; func_idx < functions[idx] -> args; func_idx++) {
                heap[*hp] = heap[functions[idx] -> hp + func_idx];
                (*hp)++;
            }
        }
}

static int
add_heap_instructions(const char** input, char delimiter, double* heap, int* hp, udf* functions[], int udf_size)
{
    char arg[100];
    int char_elapsed = 0;
    int instructions_num = 0;

    while(**input != delimiter)
    {
        if(**input == ' ') {
            (*input)++;
            continue;
        }

        instructions_num++;
        if(is_char_num(**input)) {
            double val;
            sscanf(*input, "%lg%n", &val, &char_elapsed);
            heap[(*hp)++] = val;
        }
        else if(**input == '[') {
            printf("FUCK");

        } else {
            sscanf(*input, "%s%n", arg, &char_elapsed);
            *input += char_elapsed;
            double nanbox = process_func(arg);

            if(nanbox == USERDEF) {
                write_udf(heap, hp, functions, udf_size, arg);
            }
            else
                heap[(*hp)++] = nanbox;
        }
        (*input)++;
    }
    heap[(*hp)++] = DELIMITER;
    return instructions_num;
}

static udf*
makeudf(const char** input, heap_struct* heap, udf_struct* udfs) {

    *input += 2;
    int pos = heap -> hp;

    int len = find_len(*input,' ');
    char* name = malloc(sizeof(*name) * (len + 1)); 
    memcpy(name, *input, sizeof(*name) * (len + 1));
    name[len] = '\0';
    *input += len;

    int args = add_heap_instructions(input, ';', heap -> arr, &heap -> hp, udfs -> functions, udfs -> udf_count);

    udf* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (udf) {.args = args, .name = name, .name_strlen = len, .hp = pos};

    return new_udf;
}

int 
parseInput(double* call, const char* sp, heap_struct* heap, udf_struct* udfs) {
    udf** functions = udfs -> functions;

    double val; // Place Values Used For Parsing 
    int char_elapsed;
    char arg[100];
    int call_size = 0;

    while(*sp != '\0') {

        if(*sp == ' ') {
            sp++;
            continue; 
        } 

        if(*sp == '[') {

            sp++;
            call[call_size++] = makeBox(heap -> hp, QUOTATION);
            add_heap_instructions(&sp, ']', heap -> arr, &heap -> hp, functions, udfs -> udf_count);
            heap -> arr[heap -> hp++] = DELIMITER;

        } 
        if(*sp == ':') {

            if(udfs -> udf_lim == udfs -> udf_count) {
                udfs -> udf_lim += 10;
                functions = realloc(functions, sizeof(*functions) * udfs -> udf_lim);
                udfs -> functions = functions;
            }

            udfs -> functions[udfs -> udf_count] = makeudf(&sp, heap, udfs);
            udfs -> udf_count += 1;
        } 
        if(*sp == '"') {
            sp++;
            int word_len = find_len(sp, '\"') + 1;

            call[call_size++] = makeBox(heap -> hp, STRING);
            add_string(heap -> arr, &heap -> hp, &heap -> heap_size, &sp, word_len);
        }

        if(is_char_num(*sp)) {
            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_size++] = val;
        }
        else
        {
            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;

            double parse_result = process_func(arg);

            if(get_tag(parse_result) == COMBINATOR) {

                call[call_size++] = process_func(arg);

            }
            else if(parse_result != USERDEF)
                call[call_size++] = parse_result;
            else {
                for(int idx = 0; idx < udfs -> udf_count; idx++) {
                    if(strcmp(arg, functions[idx] -> name) == 0) {
                        call[call_size++] = makeBox(functions[idx] -> hp, USERDEF);
                    }

                }

            }

        }
    }
    return call_size;
}
