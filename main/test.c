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
    TEST_CHECK(strcmp(str, "DavidSousa \n") == 0);
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
    TEST_CHECK(strcmp(str, "50 \n") == 0);
    fclose(fp);    
}

void
test_complex(void) {
    int valid_pass;
    FILE* fp = fopen("input_files/input8.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "5 45 \n") == 0);
    fclose(fp); 
}

void
test_resize(void) {
    int valid_pass;
    FILE* fp = fopen("input_files/input9.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("input_files/input10.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("output.txt", "r");
    char str[1000];
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "5 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "12.5 \n") == 0);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "12.5 10 \n") == 0);
    fclose(fp); 
}

void 
test_quote_combs(void) {
    int valid_pass = 0;

    FILE* fp = fopen("input_files/input13.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    char str[100];
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "5 24 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "20 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "0.25 \n") == 0);
    fclose(fp); 

}

void 
test_expand_mem(void) {

    int valid_pass = 0;

    FILE* fp = fopen("input_files/input14.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

}

void test_sub_quote(void) {
    int valid_pass = 0;

    FILE* fp = fopen("input_files/input15.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    char str[100];
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "52 \n") == 0);
    fclose(fp);

}

void test_list_capture(void) {
    int valid_pass = 0;

    FILE* fp = fopen("input_files/input18.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    char str[100];
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "4 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "12 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "3 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "11 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "15 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "5 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "25 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "8 \n") == 0);
    printf("%s", str);
    fclose(fp);

}

void test_bilist(void) {
    int valid_pass = 0;

    FILE* fp = fopen("input_files/input19.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    char str[100];
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "13 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "25 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "4 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp); 
    TEST_CHECK(strcmp(str, "9 25 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "7 133 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "3 \n") == 0);
    fclose(fp);
}

void test_map(void) {
    int valid_pass = 0;

    FILE* fp = fopen("input_files/input20.txt", "r");
    FILE* output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    char str[100];
    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "120 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "16 7 \n") == 0);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "4 16 \n") == 0); 
    fclose(fp);

    fp = fopen("input_files/input21.txt", "r");
    output = fopen("output.txt", "w");
    ane(fp, &valid_pass, output);
    fclose(fp);
    fclose(output);    

    fp = fopen("output.txt", "r");
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "9 \n") == 0); 
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "2 \n") == 0); 
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "6 8 \n") == 0); 
    printf("%s\n", str);
    fgets(str, sizeof(str), fp);
    fgets(str, sizeof(str), fp);
    TEST_CHECK(strcmp(str, "4 6 15 36 \n") == 0); 
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
    {"ANE Complex", test_complex},
    {"ANE Resize", test_resize},
    {"ANE Quote Combs", test_quote_combs},
    {"ANE Expansion", test_expand_mem},
    {"ANE SUB QUOTE", test_sub_quote},
    {"ANE Bi Comb", test_bilist},
    {"ANE Map", test_map},
    {"ANE LIST", test_list_capture},
    {NULL, NULL}
};

// To add 
// users//  - append to user function list
