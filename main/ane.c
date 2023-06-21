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

static void save_funcs(int udf_count, udf* functions[], double* heap, FILE* fp) {
    fwrite(&udf_count, sizeof(int), 1, fp);
    for(int idx = 0; idx < udf_count; idx++) {
        fwrite(&(functions[idx] -> name_strlen), sizeof(int), 1, fp);
        fwrite(functions[idx] -> name, sizeof(char), functions[idx] -> name_strlen, fp);
        fwrite(&(functions[idx] -> args), sizeof(int), 1, fp);
        for(int arg_idx = 0; arg_idx < functions[idx] -> args; arg_idx++)
            fwrite(&heap[functions[idx] -> hp + arg_idx], sizeof(double), 1, fp);

    }
}

static void load_funcs(int* udf_count, udf* functions[], double* heap, int* hp, FILE* fp) {
    fread(udf_count, sizeof(int), 1, fp);
    for(int idx = 0; idx < *udf_count; idx++) {
        int name_strlen;
        fread(&name_strlen, sizeof(int), 1, fp);

        char* name = malloc(sizeof(*name) * (name_strlen + 1));
        fread(name, sizeof(*name), name_strlen, fp);
        name[name_strlen] = '\0';

        int args;
        fread(&args, sizeof(int), 1, fp);

        int heap_pos = *hp;
        for(int arg_idx = 0; arg_idx < args; arg_idx++) {
            double value;
            fread(&value, sizeof(double), 1, fp);
            heap[*hp] = value;
            *hp += 1;
        }

        udf* new_func = malloc(sizeof(*new_func));
        new_func -> name_strlen = name_strlen;
        new_func -> hp = heap_pos;
        new_func -> name = name;
        new_func -> args = args;
        
        functions[idx] = new_func;
    }
}

void
ane(FILE* stream, int* valid_pass, FILE* output) {

    double* call_array = malloc(sizeof(*call_array) * 1000); // Array For Storing The Input
    double* stack = malloc(sizeof(*stack) * 1000);

    double* heap = malloc(sizeof(*heap) * 1000);
    memset(heap, '\0', 8000);
    int hp = 0; // points to the last unfilled element in the array
    int heap_size = 1000;

    int sp = -1;
 
    udf** functions = malloc(sizeof(*functions) * 10);
    int udf_count = 0;
    int udf_lim = 10;

    int size = 0; 
    int max = 0;
    for(char ch = fgetc(stream); ch != EOF; ch = fgetc(stream)) {
        if(ch == '\n') {
            if(size > max)
                max = size;
            size = 0;
        }
        else
            size++;
    } 
    fseek(stream, 0, SEEK_SET);

    char* str = malloc(sizeof(*str) * (max + 1));

    while(fgets(str, sizeof(*str) * (max + 1), stream) != NULL) {
        str[strcspn(str, "\n")] = '\0'; // Turning The New Line Into A NULL 
        
        // Quiting Condition Write Lowercase Q
        if(strcmp(str, "q") == 0) {
            fprintf(output, "ANE Closed Successfully");
            break;
        }

        if(strcmp(str, "writefuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "w");
            save_funcs(udf_count, functions, heap, udf_fp);
            fclose(udf_fp);
            continue;
        }

        if(strcmp(str, "loadfuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "r");
            load_funcs(&udf_count, functions, heap, &hp, udf_fp);
            fclose(udf_fp);
            continue;
        }

        int call_size = parseInput(call_array, str, &heap, &hp, &heap_size, &functions, &udf_count, &udf_lim);
        execStack(stack, call_array, call_size, &sp, heap, &hp);

        // Printing Errors Based On The Value Of Pass

        for(int idx = 0; idx <= sp; idx++) { // Writing The Output - Right Now Only Doubles And Strings

            if(is_num(stack[idx]) && call_size > 0)
                fprintf(output, "%lg ", stack[idx]);

            else if(get_tag(stack[idx]) == STRING && call_size > 0) {
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
    free(str);

    for(int idx = 0; idx < udf_count; idx++) { // the realloc is not applied to the original array
        free(functions[idx] -> name);
        free(functions[idx]);
    }

    free(functions);

    return;
}
