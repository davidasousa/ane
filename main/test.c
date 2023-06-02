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
    temp.call = parseInput("1 2 + 4 -", &size);
    int stack_size = calcStack(&temp, size);
    TEST_CHECK(stack_size == 1);
    TEST_CHECK(temp.stack[0] == -1);
    free(temp.call);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"Parse Input", test_parseInput},
    {"Stack Arithmatic Calculations", test_calcStack},
    {NULL, NULL}
};
