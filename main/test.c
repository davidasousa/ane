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
test_parseInput(void) {
    int size;
    double* testCall = parseInput("dup", &size);
    TEST_CHECK(size == 1);
    TEST_CHECK(get_tag(*testCall) == PREBUILT);
    free(testCall);

    testCall = parseInput("1 1", &size);
    TEST_CHECK(size == 2);
    TEST_CHECK(testCall[0] == 1);
    TEST_CHECK(testCall[1] == 1);
    free(testCall);

    testCall = parseInput("1 dup", &size);
    TEST_CHECK(size == 2);
    TEST_CHECK(testCall[0] == 1);
    TEST_CHECK(get_op(testCall[1]) == DUP);
    free(testCall);

    testCall = parseInput("1 swap *", &size);
    TEST_CHECK(size == 3);
    TEST_CHECK(testCall[0] == 1);
    TEST_CHECK(get_op(testCall[1]) == SWAP);
    TEST_CHECK(get_op(testCall[2]) == MULTIPLY);
    free(testCall);
}

void 
test_calcStack(void) {
    callStack temp;
    int size;
    int sp = -1;
    temp.call = parseInput("1 2 + 4 -", &size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == -1);
    free(temp.call);
}

void 
test_calcStack_prebuilt(void) {
    callStack temp;
    int size;
    int sp = -1;
    temp.call = parseInput("3.1 dup *", &size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == (3.1 * 3.1));
    free(temp.call);

    sp = -1;
    temp.call = parseInput("3.1 dup * 1 swap", &size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 1);
    TEST_CHECK(temp.stack[0] == 1);
    TEST_CHECK(temp.stack[1] == (3.1 * 3.1));
    free(temp.call);

    sp = -1;
    temp.call = parseInput("3.1 dup * 1 swap zap", &size);
    calcStack(&temp, size, &sp);
    TEST_CHECK(sp == 0);
    TEST_CHECK(temp.stack[0] == 1);
    free(temp.call);
}

void
test_udf(void) {
    int size;
    callStack temp;
    temp.call = parseInput("sq : dup *", &size);
    free(temp.call);
}

void
test_wrapper(void) { // For Display Of How This Will Work In A Real User Based Environment 
    int valid_pass;
    FILE* fp = fopen("input.txt", "r");
    ane(fp, &valid_pass);
    fclose(fp);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"Parse Input", test_parseInput},
    {"Stack Arithmatic Calculations", test_calcStack},
    {"Stack Prebuilt", test_calcStack_prebuilt},
    {"UDF BULDING", test_udf},
    {"Test ANE wrapper", test_wrapper},
    {NULL, NULL}
};
