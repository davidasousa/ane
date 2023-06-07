#include "acutest.h"
#include "ane.h"

void 
test_nanbox(void) {
    double nanbox = makeBox(MINUS, MATH_OPERATION);
    uint32_t op = get_op(nanbox);
    uint32_t tag = get_tag(nanbox);
    TEST_CHECK(op == MINUS);
    TEST_CHECK(tag == MATH_OPERATION);
    nanbox = makeBox(ZAP, PREBUILT);
    op = get_op(nanbox);
    tag = get_tag(nanbox);
    TEST_CHECK(op == ZAP);
    TEST_CHECK(tag == PREBUILT);
}

void
test_errors(void) { 
    int valid_pass;
    FILE* fp = fopen("input_files/error.txt", "r");
    FILE* output = fopen("output_files/error.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output_files/error.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    str[strcspn(str, "\n")] = '\0';
    TEST_CHECK(strcmp(str, "Error: Not Enough Values In Stack To Pop") == 0);
    fclose(fp);
}

void
test_ane(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input.txt", "r");
    FILE* output = fopen("output_files/output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output_files/output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "8 11 \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "0.727273 \n") == 0);
    fclose(fp);
}

void
test_ane_strings(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input3.txt", "r");
    FILE* output = fopen("output_files/output3.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"Errors", test_errors},
    {"ANE", test_ane},
    {"ANE Strings", test_ane_strings},
    {NULL, NULL}
};
