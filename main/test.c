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
    testCall = parseInput("dup 1", &size);
    TEST_CHECK(get_tag(testCall[0]) == PREBUILT);
    TEST_CHECK(testCall[1] == 1);
    free(testCall);
}

TEST_LIST = {
    {"Nanbox Operations", test_nanbox},
    {"Parse Input", test_parseInput},
    {NULL, NULL}
};
