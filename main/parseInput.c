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
    [STRCAT] = "strcat"
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
    return ERROR;
}

static UDF*
makeUDF(const char** input) {

    int char_elapsed = 0;
    int arg_count = 0; 

    char name[100];
    
    sscanf(*input, ": %s %n", name, &char_elapsed);
    name[char_elapsed] = '\0';
    *input += char_elapsed;

    for(const char* ch = *input; *ch != ';'; ch++)
        if(*ch == ' ')
            arg_count++;

    char* malloced_name = malloc(sizeof(*malloced_name) * (strlen(name) + 1));
    memcpy(malloced_name, name, sizeof(char) * (strlen(name) + 1));

    UDF* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (UDF) {.name_strlen = strlen(name) + 1, .args = arg_count, .name = malloced_name};

    double* contents = malloc(sizeof(*contents) * arg_count);
    for(int cont_idx = 0; cont_idx < arg_count; cont_idx++) {
        char* args = malloc(sizeof(*args) * 100);
        sscanf(*input, "%s %n", args, &char_elapsed);
        *input += char_elapsed;

        if(*args >= '0' && *args <= '9'){
            double num = 0;
            sscanf(args, "%lg", &num);
            contents[cont_idx] = num;
        } else
            contents[cont_idx] = process_func(args);
        free(args);
    }
    *input += 2;

    new_udf -> function = contents;

    return new_udf;
}


// finish udfs
// do some error testing
// "start quotations ..?"

void
read_string(char* strings, int* strings_pos, const char** sp) {
    *sp += 1;
    int length = 0;
    while(*(*sp + length) != '\"')
        length++;


    char* word = malloc(sizeof(*word) * (length + 1));
    strncpy(word, *sp, length);
    word[length] = '\0';

    strcpy(strings + *strings_pos, word);

    *strings_pos += length + 1;
    *sp += length + 1;
    free(word);
    return;
}

static void
save_udfs(UDF* udfs[], int udf_count, FILE* fp) {
    
    fwrite(&udf_count, sizeof(udf_count), 1, fp);
    for(int udf_idx = 0; udf_idx < udf_count; udf_idx++) {
        char* name = udfs[udf_idx] -> name;
        int args = udfs[udf_idx] -> args;
        int strlen = udfs[udf_idx] -> name_strlen;
        double* function = udfs[udf_idx] -> function;

        fwrite(&strlen, sizeof(strlen), 1, fp);
        fwrite(name, sizeof(*name), strlen, fp);
        fwrite(&args, sizeof(args), 1, fp);
        fwrite(function, sizeof(function), args, fp);

        printf("name %s args %d strlen %d", name, args, strlen);
        printf("sizeof name %lu", sizeof(name));
    }
    return;
}

static void
load_udfs(UDF* udfs[], int* udf_count, FILE* fp) {
    fread(udf_count, sizeof(*udf_count), 1, fp);

    for(int idx = 0; idx < *udf_count; idx++) {
        int strlen;
        int args;

        fread(&strlen, sizeof(int), 1, fp);

        char* name = malloc(sizeof(*name) * strlen);

        fread(name, sizeof(*name), strlen, fp);
        fread(&args, sizeof(int), 1, fp);

        double* function = malloc(sizeof(*function) * args);

        fread(function, sizeof(*function), args, fp);

        UDF* new_udf = malloc(sizeof(*new_udf));
        *new_udf = (UDF){.args = args, .name_strlen = strlen, .function = function, .name = name};
        udfs[idx] = new_udf;
    }
    return;
}

void
parseInput(double* call, const char* input, int* call_size, UDF* udfs[], int* udf_count, char strings[], int* string_pos) {
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

            udfs[(*udf_count)++] = makeUDF(&sp);  // Processing The Udf Unitl The ;
                                                  
        } else if(*sp == '"') {

            call[call_pointer++] = makeBox(*string_pos, STRING); // Add String nanbox to stack pointing to the first letter of the new string
            read_string(strings, &(*string_pos), &sp);

        } else if(*sp >= '0' && *sp <= '9') {

            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer++] = val;

        } else { // The thing in the call array is not recognizable by a prebuilt 

            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;
            
            if(strcmp(arg, "writefuncs") == 0) {
                FILE* udf_fp = fopen("udfs.txt", "w");
                save_udfs(udfs, *udf_count, udf_fp);
                fclose(udf_fp);
            }

            if(strcmp(arg, "loadfuncs") == 0) {
                FILE* udf_fp = fopen("udfs.txt", "r");
                load_udfs(udfs, udf_count, udf_fp);
                fclose(udf_fp);
            }

                double parse_result = process_func(arg);
                if(parse_result != ERROR) // Not A UDF
                    call[call_pointer++] = parse_result;

                if(parse_result == ERROR) { // udf === -1
                                            //
                    for(int udf_idx = 0; udf_idx < *udf_count; udf_idx++) {

                        if(strcmp(arg, udfs[udf_idx] -> name) == 0) {
                            double* function = udfs[udf_idx] -> function;
                            for(int func_idx = 0; func_idx < udfs[udf_idx] -> args; func_idx++) {
                                call[call_pointer++] = function[func_idx];
                            }

                        }
                        
                    }                                        
                }
        }
    }
    *call_size = call_pointer;
    return; 
}
