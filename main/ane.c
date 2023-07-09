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
    for(int udf_idx = 0; udf_idx < udf_count; udf_idx++) {
        fwrite(&functions[udf_idx] -> name_strlen, sizeof(int), 1, fp);

        fwrite(functions[udf_idx] -> name, sizeof(char), functions[udf_idx] -> name_strlen, fp);
        fwrite(&functions[udf_idx] -> args, sizeof(int), 1, fp);
        fwrite(&heap[functions[udf_idx] -> hp], sizeof(double) * functions[udf_idx] -> args, 1, fp);

    }
}

static void load_funcs(int* udf_count, udf* functions[], double* heap, int* hp, FILE* fp) {
    fread(udf_count, sizeof(int), 1, fp);
    for(int udf_idx = 0; udf_idx < *udf_count; udf_idx++) {
        int name_strlen;
        fread(&name_strlen, sizeof(int), 1, fp);

        char* name = malloc(sizeof(*name) * (name_strlen + 1));
        fread(name, sizeof(char), name_strlen, fp);
        name[name_strlen] = '\0';
        
        int args;
        fread(&args, sizeof(int), 1, fp);

        int heap_pos = *hp;
        for(int idx = 0; idx < args; idx++) {
            fread(&heap[(*hp)++], sizeof(double), 1, fp);
        }
        heap[(*hp)++] = DELIMITER;

        udf* new_udf = malloc(sizeof(*new_udf));
        *new_udf = (udf){.hp = heap_pos, .name = name, .name_strlen = name_strlen, .args = args}; 

        functions[udf_idx] = new_udf;
    }

}

static int
find_max_line_size(FILE* stream) {
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
    return max;
}


void
ane(FILE* stream, int* valid_pass, FILE* output) {

    double* call_array = malloc(sizeof(*call_array) * 1000); // Array For Storing The Input
    
    stack_struct* stack = malloc(sizeof(*stack));
    *stack = (stack_struct) {.stack = malloc(sizeof(*stack) * 1000), .sp = -1, .stack_size = 1000};

    heap_struct* heap = malloc(sizeof(*heap));
    *heap = (heap_struct) {.arr = malloc(sizeof(double) * 1000), .heap_size = 1000, .hp = 0};
    memset(heap -> arr, '\0', 8000);

    udf_struct* udfs = malloc(sizeof(*udfs));
    *udfs = (udf_struct) {.functions = malloc(sizeof(udf) * 10), .udf_lim = 10, .udf_count = 0};

    int max = find_max_line_size(stream);
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
            FILE* udf_fp = fopen("udfs.txt", "wb");
            save_funcs(udfs -> udf_count, udfs -> functions, heap -> arr, udf_fp);
            fclose(udf_fp);
            continue;
        }

        if(strcmp(str, "loadfuncs") == 0) {
            FILE* udf_fp = fopen("udfs.txt", "rb");
            load_funcs(&udfs -> udf_count, udfs -> functions, heap -> arr, &heap -> hp, udf_fp);
            fclose(udf_fp);
            continue;
        }

        // Running Each Line

        int call_size = parseInput(call_array, str, heap, udfs);
        execStack(&stack -> stack, call_array, call_size, &stack -> sp, &stack -> stack_size, heap);

        // Writing The Result Output

        for(int idx = 0; idx <= stack -> sp; idx++) {

            if(get_tag(stack -> stack[idx] == LIST)) // hard coded because the list was printing as a crazy number when it shouldnt be printing at all
                continue;

            if(is_num(stack -> stack[idx]) && call_size > 0)
                fprintf(output, "%lg ", stack -> stack[idx]);

            else if(get_tag(stack -> stack[idx]) == STRING && call_size > 0) {
                fprintf(output, "%s ", (char*) &heap -> arr[get_op(stack -> stack[idx])]);

            }
        }

        if(call_size)
            fprintf(output, "\n");

    }

    //Freeing Used Arrays
    free(stack -> stack);
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
