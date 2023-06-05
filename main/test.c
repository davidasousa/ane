#include "acutest.h"
#include "ane.h"

void 
test_nanbox(void) {
    double nanbox = makeBox(MINUS, OPERATION);
    uint32_t op = get_op(nanbox);
    uint32_t tag = get_tag(nanbox);
    TEST_CHECK(op == MINUS);
    TEST_CHECK(tag == OPERATION);
    nanbox = makeBox(ZAP, PREBUILT);
    op = get_op(nanbox);
    tag = get_tag(nanbox);
    TEST_CHECK(op == ZAP);
    TEST_CHECK(tag == PREBUILT);
}

void 
test_calcStack(void) {
    int udf_size = 0;
    UDF* udfs[10];
    callStack temp;
    int size;
    int sp = -1;
    temp.call = parseInput("1 2 + 4 -", &size, udfs, &udf_size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == -1);
    free(temp.call);
}

void 
test_calcStack_prebuilt(void) {
    int udf_size = 0;
    UDF* udfs[10];
    callStack temp;
    int size;
    int sp = -1;
    temp.call = parseInput("3.1 dup *", &size, udfs, &udf_size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == (3.1 * 3.1));
    free(temp.call);

    sp = -1;
    temp.call = parseInput("3.1 dup * 1 swap", &size, udfs, &udf_size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 1);
    TEST_CHECK(temp.stack[0] == 1);
    TEST_CHECK(temp.stack[1] == (3.1 * 3.1));
    free(temp.call);

    sp = -1;
    temp.call = parseInput("3.1 dup * 1 swap zap", &size, udfs, &udf_size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == 1);
    free(temp.call);
}

void
test_udf_building(void) {
    int size;
    callStack temp;
    int udf_size = 0;
    UDF* udfs[10];
    temp.call = parseInput(": sq dup * ;", &size, udfs, &udf_size);
    TEST_CHECK(udfs[0] -> args == 2);
    TEST_CHECK(strcmp(udfs[0] -> name, "sq") == 0);
    free(udfs[0] -> function);
    free(udfs[0]);
    free(temp.call);
}

void
test_wrapper(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input_files/input.txt", "r");
    ane(fp, &valid_pass);
    fclose(fp);
    fp = fopen("input_files/input2.txt", "r");
    ane(fp, &valid_pass);
    fclose(fp);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"Stack Arithmatic Calculations", test_calcStack},
    {"Stack Prebuilt", test_calcStack_prebuilt},
   // {"UDF BULDING", test_udf_building},
    {"Test ANE wrapper", test_wrapper},
    {NULL, NULL}
};
