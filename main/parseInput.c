#include "ane.h"

// Functions To Build Arrays For The Operations - Prebuilts, Math, Strings

static const char* prebuilt_ops[] = {
    [DUP] = "dup",
    [SWAP] = "swap",
    [ZAP] = "zap",
    [CLEAR] = "clear"
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

static const char* combs[] = {
    [I] = "I",
    [BI] = "bi",
};

static const char* list_ops[] = {
    [LIST_LENGTH] = "#",
    [LIST_PUSH_IDX] = "#n",
    [CAT] = "cat",
    [SUM_LIST] = "/+"
};

static bool
is_char_num(char ch) { return (ch >= '0' && ch <= '9'); }

static double
process_arg(const char* input) { // Take A Given Char* Input & Associate With The Right Function

    int prebuilt_length = sizeof(prebuilt_ops) / sizeof(prebuilt_ops[0]);
    int math_length = sizeof(math_ops) / sizeof(math_ops[0]);
    int string_op_length = sizeof(string_ops) / sizeof(string_ops[0]);
    int comb_length = sizeof(combs) / sizeof(combs[0]);
    int list_length = sizeof(list_ops) / sizeof(list_ops[0]);

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
    for(int idx = 0; idx < comb_length; idx++) {
        if(strcmp(input, combs[idx]) == 0)
            return makeBox(idx, COMBINATOR);
    }

    for(int idx = 0; idx < list_length; idx++) {
        if(strcmp(input, list_ops[idx]) == 0)
            return makeBox(idx, LIST_OPERATION);
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
write_udf(double* dest, double* heap, int* hp, udf* functions[], int udf_size, char* arg) {

    for(int idx = 0; idx < udf_size; idx++)
        if(strcmp(arg, functions[idx] -> name) == 0)
            for(int func_idx = 0; func_idx < functions[idx] -> args; func_idx++) {
                dest[*hp] = heap[functions[idx] -> hp + func_idx];
                (*hp)++;
            }
}

static void
write_function(double val, char* arg, heap_struct* heap, udf_struct* udfs) { // can call udf as a function
    if(val == USERDEF)
        write_udf(heap -> arr, heap -> arr, &heap -> hp, udfs -> functions, udfs -> udf_count, arg);
    else
        heap -> arr[(heap -> hp)++] = val;
    return;
}


static void 
skip_over(const char** ch, char delim) {
    while(**ch != delim) 
        (*ch)++;
    (*ch)++;
}


static int
write_function_heap(const char** ch, char delimiter, heap_struct* heap, udf_struct* udfs) {
    char arg[100];
    int char_elapsed = 0;
    int instructions_num = 0;

    while(**ch != delimiter) {
        switch(**ch) {
            case '[' :;
                      skip_over(ch, ']');
                      break;
            case ' ' :;
                      (*ch)++;
                      break;
            default :;
                     if(is_char_num(**ch)) {
                         double val;
                         sscanf(*ch, "%lg%n", &val, &char_elapsed);
                         heap -> arr[(heap -> hp)++] = val;
                     } else {
                         sscanf(*ch, "%s%n", arg, &char_elapsed);
                         *ch += char_elapsed;
                         write_function(process_arg(arg), arg, heap, udfs);
                     }
                     (*ch)++;
                     instructions_num++;
        }
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
    int args = write_function_heap(input, ';', heap , udfs);
    heap -> arr[(heap -> hp)++] = DELIMITER;

    // Adding Udf To Udf Array
    udf* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (udf) {.args = args, .name = name, .name_strlen = len, .hp = pos};

    return new_udf;
}

static int
create_quotation(const char** ch, heap_struct* heap, udf_struct* udfs) {

    (*ch)++;
    char arg[100];
    int char_elapsed = 0;
    double val;

    int hp = heap -> hp;
    while(**ch != ']') {

        if(**ch == ' ') { // skiping spaces
            (*ch)++;
            continue;
        }

        switch(**ch) {

            case '[' :;
                      heap -> arr[(heap -> hp)] = makeBox(heap -> hp + 1, QUOTATION);
                      heap -> hp += 1;
                      create_quotation(&(*ch), heap, udfs);
                      break;
            default :;

                     if(is_char_num(**ch)) {                         // numerical arg
                         sscanf(*ch, "%lg%n", &val, &char_elapsed);
                         heap -> arr[(heap -> hp)++] = val;
                     } else {                                        // textual arg
                         sscanf(*ch, "%s%n", arg, &char_elapsed);
                         *ch += char_elapsed;
                         write_function(process_arg(arg), arg, heap, udfs);
                     }

        }
        (*ch)++;
    }

    heap -> arr[heap -> hp++] = DELIMITER;
    return hp;
}

static int
create_list(const char** ch, heap_struct* heap, udf_struct* udfs) { // requires creation of nested quoetes first and lists - fix

    (*ch)++;
    int size_pos = heap -> hp; // also the first list element
    heap -> arr[(heap -> hp)++] = 0;

    double val;
    char arg[100];
    int char_elapsed;

    while(**ch != '}') {

        if(**ch == ' ')  {
            (*ch)++;
            continue;
        }

        switch(**ch) {

            case '[':;

                     *ch += 1;
                     heap -> arr[(heap -> hp)] = makeBox((heap -> hp) + 1, QUOTATION);
                     heap -> hp++;
                     create_quotation(ch, heap, udfs);
                     break;

            case '{' :;

                      heap -> arr[(heap -> hp)] = makeBox(heap -> hp + 1, LIST);
                      (heap -> hp)++;
                      create_list(&(*ch), heap, udfs);
                      break;

            default:;

                    if(is_char_num(**ch)) {                        
                        sscanf(*ch, "%lg%n", &val, &char_elapsed);
                        heap -> arr[(heap -> hp)++] = val;
                    } else {                                       
                        sscanf(*ch, "%s%n", arg, &char_elapsed);
                        write_function(process_arg(arg), arg, heap, udfs);
                    }
                    *ch += char_elapsed;

        }

        (*ch)++;
        heap -> arr[size_pos]++;

    }

    (*ch)++;
    heap -> arr[heap -> hp++] = DELIMITER;
    return size_pos;
}

int 
parseInput(double* call, const char* sp, heap_struct* heap, udf_struct* udfs) {
    udf** functions = udfs -> functions;

    double val; // Place Values Used For Parsing 
    char arg[100];
    int call_size = 0;
    int hp;

    while(*sp != '\0') {
        switch(*sp) {
            case ' ' :;
                      sp++;
                      break;

            case '[' :;

                      sp++;
                      hp = create_quotation(&sp, heap, udfs);
                      call[call_size++] = makeBox(hp, QUOTATION);
                      break;

            case '{' :;

                      hp = create_list(&sp, heap, udfs);
                      call[call_size++] = makeBox(hp, LIST);
                      break;

            case ':' :;

                      if(udfs -> udf_lim == udfs -> udf_count) {
                          udfs -> udf_lim += 10;
                          functions = realloc(functions, sizeof(*functions) * udfs -> udf_lim);
                          udfs -> functions = functions;
                      }
                      udfs -> functions[udfs -> udf_count] = makeudf(&sp, heap, udfs);
                      udfs -> udf_count += 1;
                      break;

            case '"' :;

                      int word_len = find_len(++sp, '\"') + 1;
                      call[call_size++] = makeBox(heap -> hp, STRING);
                      add_string(heap -> arr, &heap -> hp, &heap -> heap_size, &(sp), word_len);
                      break;

            default :;

                     int char_elapsed = 0;
                     sscanf(sp, "%s%n", arg, &char_elapsed);
                     sscanf(sp, "%lg%n", &val, &char_elapsed);

                     if(is_char_num(*sp)) {
                         call[call_size++] = val;
                     }
                     else {

                         double parse_result = process_arg(arg);

                         if(parse_result != USERDEF)
                             call[call_size++] = parse_result;
                         else
                             write_udf(call, heap -> arr, &call_size, udfs -> functions, udfs -> udf_count, arg);

                     }
                     sp += char_elapsed;

        }


    }
    return call_size;
}
