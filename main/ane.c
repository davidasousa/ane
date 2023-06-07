#include "ane.h"

// QUESTION: how to test a file inputted test - testing ane full itself <-
//
// ADD: udfs within udfs 
// udf loading 
// error testing 
// fix exescive memory allocations throughout
// error messaging 

static int
is_num(double val) { return val == val;}

static void
write_string(FILE* fp, char strings[], int string_pos) {
    fprintf(fp, "%s ", strings + string_pos);
    return;
}

static void
save_udfs(UDF* udfs[], int udf_count, FILE* fp) {
    fprintf(fp, "%d\n", udf_count);
    for(int udf_idx = 0; udf_idx < udf_count; udf_idx++) {
        double* function = udfs[udf_idx] -> function;
        fprintf(fp, "%s,%d", udfs[udf_idx] -> name, udfs[udf_idx] -> args);
        for(int arg_idx = 0; arg_idx < udfs[udf_idx] -> args; arg_idx++)
            fprintf(fp, "%lu", (uint64_t)function[arg_idx]);
        fprintf(fp, "\n");
    }
}

void
ane(FILE* stream, int* valid_pass, FILE* output) {
    char str[1000];
    callStack stack;
    int sp = -1;

    UDF** udfs = malloc(sizeof(*udfs) * 10);
    int udfs_count = 0;

    char* strings = malloc(sizeof(*strings) * 100); // whole thing is one big string
    memset(strings, '\0', 100);
    int string_pos = 0;

    double* heap = malloc(sizeof(*heap) * 1000);
    int heap_size = 0;

    // Save the process into a file to be brought later
    
    while(fgets(str, sizeof(str), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0';

        if(strcmp(str, "q") == 0) {
            fprintf(output, "ANE Closed Successfully");
            break;
        }

        int call_size = 0;
        stack.call = parseInput(str, &call_size, udfs, &udfs_count, heap, &heap_size, strings, &string_pos);
        int pass = calcStack(&stack, call_size, &sp, output, strings, &string_pos);
        if(pass != 0) {
            fprintf(output,"Error: Not Enough Values In Stack To Pop");
            free(stack.call);
            break;
        }

        for(int idx = 0; idx <= sp; idx++) {
            if(is_num(stack.stack[idx]))
                fprintf(output, "%lg ", stack.stack[idx]);
            else
                write_string(output, strings, get_op(stack.stack[idx]));
        }
        fprintf(output, "\n");

        free(stack.call);
        // stack call is malloced and freed everytime- fix this FIX
    }
    FILE* fp = fopen("udfs.txt", "w"); // This can become its own function later
    save_udfs(udfs, udfs_count, fp);
    fclose(fp);

    for(int idx = 0; idx < udfs_count; idx++) {
        free(udfs[idx] -> function);
        free(udfs[idx] -> name);
        free(udfs[idx]);
    }
    
    free(udfs); free(heap); free(strings);

    return;
}
