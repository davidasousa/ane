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

void
ane(FILE* stream, int* valid_pass, FILE* output) {

    char str[1000];

    double* call_array = malloc(sizeof(*call_array) * 1000); // Array For Storing The Input
    double* stack = malloc(sizeof(*stack) * 1000);

    int sp = -1;

    double* heap = malloc(sizeof(*heap) * 100);
    int hp = 0; // points to the last unfilled element in the array
    
    udf** functions = malloc(sizeof(*functions) * 10);
    int udf_count = 0;

    while(fgets(str, sizeof(str), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0'; // Turning The New Line Into A NULL 
        
        // Quiting Condition Write Lowercase Q
        if(strcmp(str, "q") == 0) {
            fprintf(output, "ANE Closed Successfully");
            break;
        }

        if(strcmp(str, "writefuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "w");
            fclose(udf_fp);
            continue;
        }

        if(strcmp(str, "loadfuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "r");
            fclose(udf_fp);
            continue;
        }
        
        int call_size;

        parseInput(call_array, str, &call_size, heap, &hp, functions, &udf_count);
        execStack(stack, call_array, call_size, &sp, heap, &hp);

        // Printing Errors Based On The Value Of Pass

        for(int idx = 0; idx < sp + 1; idx++) { // Writing The Output - Right Now Only Doubles And Strings
            if(is_num(stack[idx]) && call_size > 0)
                fprintf(output, "%lg ", stack[idx]);
            else if(get_tag(stack[idx]) == STRING) {
                fprintf(output, "%s ", (char*) &heap[get_op(stack[idx])]);
            }
        }

        if(call_size)
            fprintf(output, "\n");

        // End Writing 
    }

    //Freeing Used Arrays
    free(stack); 
    free(call_array);
    free(heap);

    for(int idx = 0; idx < udf_count; idx++) {
        free(functions[idx] -> name);
        free(functions[idx]);
    }
    
    free(functions);

    return;
}
