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
test_ane_arithmatic(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "10 \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "8 \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "32 \n") == 0);
    fclose(fp);
}

void
test_ane_strings(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input2.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "David Sousa \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "DavidSousa \n") == 0); // strcat strlen
    fclose(fp);
}

void
test_create_udf(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input3.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "16 \n") == 0); // 4 sq
    fclose(fp);

    fp = fopen("input_files/input4.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "10 \n") == 0); // 4 sq half
    fclose(fp);
}

void
test_udf_in_udf(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input6.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);
    
    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "2.25 \n") == 0);
    fclose(fp);

    fp = fopen("input_files/input7.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);

    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "20 \n") == 0);
    fclose(fp);
}

void
test_save_load_udf(void) {
    int valid_pass;
    FILE* fp = fopen("input_files/input5.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("input_files/input5b.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "24 24 \n") == 0);
    printf("%s", str);
    fclose(fp);

    
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"ANE Basic Arithmatic", test_ane_arithmatic},
    {"ANE Basic Strings", test_ane_strings},
    {"ANE Basic UDF Creation", test_create_udf},
    {"ANE Udf In Udf", test_udf_in_udf},
    {"ANE Udf Save/Load", test_save_load_udf},
    {NULL, NULL}
};

// To add 
// users//  - append to user function list
