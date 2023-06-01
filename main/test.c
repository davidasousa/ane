#include "acutest.h"
#include "ane.h"

void 
test_nanbox(void) {
    double nanbox = makeBox(MINUS, OPERATION);
    uint32_t tag = get_op(nanbox);
    TEST_CHECK(tag == MINUS);
}

TEST_LIST = {
    {"Test Nanbox Operations", test_nanbox},
    {NULL, NULL}
};
