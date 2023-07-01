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

static bool
is_char_num(char ch) { return (ch >= '0' && ch <= '9'); }

static double
process_arg(const char* input) { // Take A Given Char* Input & Associate With The Right Function

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

static void 
skip_over(const char** ch, char delim) {
    while(**ch != delim) 
        (*ch)++;
    (*ch)++;
}


    static int
write_function_heap(const char** ch, char delimiter, heap_struct* heap, udf* functions[], int udf_size)
{
    char arg[100];
    int char_elapsed = 0;
    int instructions_num = 0;

    while(**ch != delimiter)
    {

        if(**ch == '[')
            skip_over(ch, ']');

        if(**ch == ' ') {
            (*ch)++;
            continue;
        }
        if(is_char_num(**ch)) {
            double val;
            sscanf(*ch, "%lg%n", &val, &char_elapsed);
            heap -> arr[(heap -> hp)++] = val;
        } else {
            sscanf(*ch, "%s%n", arg, &char_elapsed);
            *ch += char_elapsed;
            double nanbox = process_arg(arg);

            if(nanbox == USERDEF) {
                write_udf(heap -> arr, &heap -> hp, functions, udf_size, arg);
            }
            else
                heap -> arr[(heap -> hp)++] = nanbox;
        }
        (*ch)++;
        instructions_num++;
    }
    heap -> arr[heap -> hp++] = DELIMITER;
    return instructions_num;
}

static udf*
makeudf(const char** input, heap_struct* heap, udf_struct* udfs) {

    *input += 2;
    int pos = heap -> hp;

    // Reading Name
    int len = find_len(*input,' ');
    char* name = malloc(sizeof(*name) * (len + 1)); 
    memcpy(name, *input, sizeof(*name) * (len + 1));
    name[len] = '\0';
    *input += len;

    // Writing Function Contents To Mem
    (*input)++;
    int args = write_function_heap(input, ';', heap , udfs -> functions, udfs -> udf_count);
    heap -> arr[(heap -> hp)++] = DELIMITER;

    // Adding Udf To Udf Array
    udf* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (udf) {.args = args, .name = name, .name_strlen = len, .hp = pos};

    return new_udf;
}

static int
create_quotation(const char** ch, heap_struct* heap, udf_struct* udfs, int udf_size) {
    const char* start_pos = *ch;
    int* quotes = malloc(sizeof(*quotes) * 10);
    int idx = 0;

    for( ; **ch != ']'; (*ch)++) // Recursive Case - QUOTATIONS ARE MADE OUTWARD -> Bigger Quotations Later
        if(**ch == '[') {
            (*ch)++;
            quotes[idx++] = create_quotation(ch, heap, udfs, udf_size);
        }

    *ch = start_pos + 1;

    char arg[100];
    int char_elapsed = 0;
    int hp = heap -> hp;
    idx = 0;
    double val;

    while(**ch != ']')
    {
        switch(**ch) {

            case '[' :;
                heap -> arr[(heap -> hp)++] = makeBox(quotes[idx++], QUOTATION);
                skip_over(ch, ']');
                break;
            case ' ' :;
                (*ch)++;
                continue;
            default:;

                if(is_char_num(**ch)) {                         // numerical arg
                    sscanf(*ch, "%lg%n", &val, &char_elapsed);
                    heap -> arr[(heap -> hp)++] = val;
                } else {                                        // textual arg
                    sscanf(*ch, "%s%n", arg, &char_elapsed);
                    *ch += char_elapsed;
                    val = process_arg(arg);

                    if(val == USERDEF) {
                        write_udf(heap -> arr, &heap -> hp, udfs -> functions, udfs -> udf_count, arg);
                    }
                    else {
                        heap -> arr[(heap -> hp)++] = val;
                    }
                }

        }
        (*ch)++;
    }

    heap -> arr[heap -> hp++] = DELIMITER;
    free(quotes);
    return hp;
}

int 
parseInput(double* call, const char* sp, heap_struct* heap, udf_struct* udfs) {
    udf** functions = udfs -> functions;

    double val; // Place Values Used For Parsing 
    int char_elapsed;
    char arg[100];
    int call_size = 0;

    while(*sp != '\0') {
        switch(*sp) {
            case ' ' :
                sp++;
                break;

            case '[' :;

                sp++;
                int hp = create_quotation(&sp, heap, udfs, udfs -> udf_count);
                call[call_size++] = makeBox(hp, QUOTATION);
                break;

            case ':' :;

                // CHECK UDF LIM REALLOC
                if(udfs -> udf_lim == udfs -> udf_count) {
                    udfs -> udf_lim += 10;
                    functions = realloc(functions, sizeof(*functions) * udfs -> udf_lim);
                    udfs -> functions = functions;
                }
                // END UDF LIM REALLOC 
                udfs -> functions[udfs -> udf_count] = makeudf(&sp, heap, udfs);
                udfs -> udf_count += 1;
                break;

            case '"' :;
                int word_len = find_len(++sp, '\"') + 1;
                call[call_size++] = makeBox(heap -> hp, STRING);
                add_string(heap -> arr, &heap -> hp, &heap -> heap_size, &(sp), word_len);
                break;

            default:

                if(is_char_num(*sp)) {
                    sscanf(sp, "%lg%n", &val, &char_elapsed);
                    sp += char_elapsed;
                    call[call_size++] = val;
                }
                else 
                {
                    sscanf(sp, "%s%n", arg, &char_elapsed);
                    sp += char_elapsed;

                    double parse_result = process_arg(arg);

                    if(parse_result != USERDEF)
                        call[call_size++] = parse_result;
                    else
                        for(int idx = 0; idx < udfs -> udf_count; idx++)
                            if(strcmp(arg, functions[idx] -> name) == 0)
                                call[call_size++] = makeBox(functions[idx] -> hp, USERDEF);
                }
// end
        }


    }
    return call_size;
}
