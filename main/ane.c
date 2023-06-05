#include "ane.h"

// QUESTION: how to test a file inputted test - testing ane full itself <-
//
// ADD: udfs within udfs 
// udf loading 
// error testing 
// fix exescive memory allocations throughout
// error messaging 

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

// load udfs

double* 
ane(FILE* stream, int* valid_pass) {
    char str[1000];
    callStack stack;
    int sp = -1;

    UDF** udfs = malloc(sizeof(*udfs) * 10);
    int udfs_count = 0;

    // Save the process into a file to be brought later
    
    while(fgets(str, sizeof(str), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0';

        if(strcmp(str, "q") == 0)
            break;

        int call_size = 0;
        stack.call = parseInput(str, &call_size, udfs, &udfs_count);
        calcStack(&stack, call_size, &sp);

        for(int idx = 0; idx <= sp; idx++)
            printf("%lg ", stack.stack[idx]);
        printf("\n");
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
    free(udfs);

    printf("ANE Closed Successfully");

    return NULL;
}
