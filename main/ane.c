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

    heap_struct* heap = malloc(sizeof(*heap));
    *heap = (heap_struct) {.arr = malloc(sizeof(double) * 1000), .heap_size = 1000, .hp = 0};
    memset(heap -> arr, '\0', 8000);

    udf_struct* udfs = malloc(sizeof(*udfs));
    *udfs = (udf_struct) {.functions = malloc(sizeof(udf) * 10), .udf_lim = 10, .udf_count = 0};

    int sp = -1;
 
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
            save_funcs(udfs -> udf_count, udfs -> functions, heap -> arr, udf_fp);
            fclose(udf_fp);
            continue;
        }

        if(strcmp(str, "loadfuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "r");
            load_funcs(&udfs -> udf_count, udfs -> functions, heap -> arr, &heap -> hp, udf_fp);
            fclose(udf_fp);
            continue;
        }

        // Running Each Line

        int call_size = parseInput(call_array, str, heap, udfs);
        execStack(stack, call_array, call_size, &sp, heap);

        for(int idx = 0; idx <= sp; idx++) {

            if(is_num(stack[idx]) && call_size > 0)
                fprintf(output, "%lg ", stack[idx]);

            else if(get_tag(stack[idx]) == STRING && call_size > 0) {
                fprintf(output, "%s ", (char*) &heap -> arr[get_op(stack[idx])]);

            }
        }

        if(call_size)
            fprintf(output, "\n");

    }

    //Freeing Used Arrays
    free(stack); 
    free(call_array);
    free(heap -> arr);
    free(heap);
    free(str);

    for(int idx = 0; idx < udfs -> udf_count; idx++) { // the realloc is not applied to the original array
        free(udfs -> functions[idx] -> name);
        free(udfs -> functions[idx]);
    }

    free(udfs -> functions);
    free(udfs);

    return;
}
