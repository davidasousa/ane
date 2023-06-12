#include "ane.h"

// QUESTION: how to test a file inputted test - testing ane full itself <-
//
// ADD: udfs within udfs 
// udf loading 
// error testing 
// fix exescive memory allocations throughout
// error messaging 

int
is_num(double val) { return val == val;}

static void
write_string(FILE* fp, char strings[], int string_pos) {
    fprintf(fp, "%s ", strings + string_pos);
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
ane(FILE* stream, int* valid_pass, FILE* output) {

    char str[1000];

    double* call_array = malloc(sizeof(*call_array) * 1000); // Array For Storing The Input
    double* stack = malloc(sizeof(*stack) * 1000);
    UDF** udfs = malloc(sizeof(*udfs) * 10);

    int sp = -1;
    int udfs_count = 0;

    char* strings = malloc(sizeof(*strings) * 100); // whole thing is one big string
    int string_pos = 0;

    // Save the process into a file to be brought later

    while(fgets(str, sizeof(str), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0'; // Turning The New Line Into A NULL 
        
        // Quiting Condition Write Lowercase Q
        if(strcmp(str, "q") == 0) {
            fprintf(output, "ANE Closed Successfully");
            break;
        }

        if(strcmp(str, "writefuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "w");
            save_udfs(udfs, udfs_count, udf_fp);
            fclose(udf_fp);
            continue;
        }

        if(strcmp(str, "loadfuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "r");
            load_udfs(udfs, &udfs_count, udf_fp);
            fclose(udf_fp);
            continue;
        }

        int call_size;

        parseInput(call_array, str, &call_size, udfs, &udfs_count, strings, &string_pos);
        execStack(stack, call_array, call_size, &sp, output, strings, &string_pos);

        // Printing Errors Based On The Value Of Pass

        for(int idx = 0; idx < sp + 1; idx++) { // Writing The Output - Right Now Only Doubles And Strings
            if(is_num(stack[idx]) && call_size > 0)
                fprintf(output, "%lg ", stack[idx]);

            if(get_tag(stack[idx]) == STRING)
                write_string(output, strings, get_op(stack[idx]));
        }

        if(call_size)
            fprintf(output, "\n");

        // End Writing 
    }

    // Freeing Udfs
    for(int idx = 0; idx < udfs_count; idx++) {
        free(udfs[idx] -> function);
        free(udfs[idx] -> name);
        free(udfs[idx]);
    }
    
    //Freeing Used Arrays
    free(udfs); 
    free(strings); 
    free(stack); 
    free(call_array);

    return;
}
