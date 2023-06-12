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
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
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
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "2 \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "4 \n") == 0);
    fclose(fp);
}

void
test_ane_strings(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input3.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);

    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "David Sousa \n") == 0);
    fgets(str, sizeof(str), fp); // running strcat and strlen
    TEST_CHECK(strcmp(str, "DavidSousa 10 \n") == 0);
    fclose(fp);
    
    // Another Input File

    fp = fopen("input_files/input4.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);

    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "New String 1 New String 2 \n") == 0);
    fclose(fp);
}

void
test_read_write_udf(void) {
    int valid_pass;
    FILE* fp = fopen("input_files/input2.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);

    fp = fopen("input_files/input2_load.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);

    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "32 \n") == 0); // 4 quad double in combo function
    fclose(fp);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"ANE", test_ane},
    {"ANE Strings", test_ane_strings},
    {"ANE Read & Write UDFs", test_read_write_udf},
    {NULL, NULL}
};
