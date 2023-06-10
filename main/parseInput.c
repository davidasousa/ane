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

static UDF*
makeUDF(const char** input, UDF* udfs[], int* udf_count) {
    *input += 2;

    // Finding The Name & Name Length
    int name_len = 0;
    char* name = malloc(sizeof(*name) * 100);

    while(**input != ' ') {
        name[name_len++] = **input;
        *input += 1;
    }
    name[name_len++] = '\0';

    // Finding The Function & Argument Count
    double* function = malloc(sizeof(*function) * 100); // malloced for resize later
    int func_size = 0;

    while(**input != ';') {
        if(**input == ' ') {
            *input += 1;
            continue;
        }

        int char_elapsed;
        char arg[100]; 
        sscanf(*input,"%s%n", arg, &char_elapsed);

        if(arg[0] >= '0' && arg[0] <= '9'){
            double val;
            sscanf(*input, "%lg", &val);
            function[func_size++] = val;
        } else if (process_func(arg) != USERDEF) {
            function[func_size++] = process_func(arg);
        } else {

            UDF* curr_udf;
            for(int udf_idx = 0; udf_idx < *udf_count; udf_idx++) {
                if(strcmp(udfs[udf_idx] -> name, arg) == 0)
                    curr_udf = udfs[udf_idx];
            }

            for(int udf_func_idx = 0; udf_func_idx < curr_udf -> args; udf_func_idx++) {
                function[func_size++] = (curr_udf -> function)[udf_func_idx];
            }

        }
        *input += char_elapsed;
    }

    function = (double*) realloc(function, sizeof(*function) * func_size);
    name = (char*) realloc(name, sizeof(*name) * name_len);

    UDF* new_udf = malloc(sizeof(*new_udf));
    *new_udf = (UDF){.name = name, .name_strlen = name_len, .function = function, .args = func_size};

    return new_udf;
}

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

            udfs[(*udf_count)] = makeUDF(&sp, udfs, udf_count);  // Processing The Udf Unitl The ;
            *udf_count += 1;

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
            if(parse_result != USERDEF) // Not A UDF
                call[call_pointer++] = parse_result;

            if(parse_result == USERDEF) { // udf === -1
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
