#include "acutest.h"
#include "ane.c"

void
test_basic_add(void)
{
    int pass;
    double* ans = ane("1 2 +", &pass);
    TEST_CHECK(ans[0] == 3);
    TEST_CHECK(pass == EXIT_SUCCESS);
    free(ans);
}

void
test_empty_input(void)
{
    int pass;
    ane("", &pass);
    TEST_CHECK(pass == EXIT_FAILURE);
}

void
test_one_input(void)
{
    int pass;
    ane("1", &pass);
    TEST_CHECK(pass == EXIT_FAILURE);
}
/* Should Multiple Final Answers Be Left On The Stack
void
test_multiple_stack(void)
{
    int pass;
    double ans = ane("1 2 + 3 1 -", &pass);
    TEST_CHECK(ans == 3);
    TEST_CHECK(pass == EXIT_FAILURE);
}
*/

void
test_complex_arithmatic(void)
{
    int pass;
    double* ans = ane("1.23131 0.4211 / 1.411 +", &pass);
    TEST_CHECK(*ans > 4.3350322963); // REFERENCE - AROUND WHERE THE DECIMALS DONT MATCH UP
    TEST_CHECK(*ans < 4.3350322965);
    TEST_CHECK(pass == EXIT_SUCCESS);
    free(ans);
}

void
test_dup(void)
{
    int pass;
    double* ans = ane("1 dup +", &pass);
    TEST_CHECK(*ans == 2);
    TEST_CHECK(pass == EXIT_SUCCESS);
    free(ans);
}

void
test_swap(void)
{
    int pass;
    double* ans = ane("1 2 swap", &pass);
    TEST_CHECK(ans[0] == 2);
    TEST_CHECK(pass == EXIT_SUCCESS);
    free(ans);
}


void
test_zap(void)
{
    int pass;
    double* ans = ane("1 2 zap", &pass);
    TEST_CHECK(ans[0] == 1);
    //TEST_CHECK(ans[1] == 0);
    TEST_CHECK(pass == EXIT_SUCCESS);
    free(ans);
}



TEST_LIST = {
    {"Basic Add", test_basic_add},
    {"Empty Input", test_empty_input},
    {"One Input", test_one_input},
    //{"Multiple Answers", test_multiple_stack},
    {"Complex Arithmatic", test_complex_arithmatic},
    {"Dup Function", test_dup},
    {"Zap Function", test_zap},
    {NULL, NULL}
};

// Questions: 
//
// 1. More Specific Math Library?
//
// 2. How To Organize The Builtin Functions/ Operations & Execute Them?
//
// 3. Two Seperate Tags Builtins & UDFs?
//
//
// Errors:
// Pop Non existant values
// Only one value should be on stack
//
// Shell Script
// Acutest
// 
// Function Definition For ^2
// : sq dup * ; 9.5 sq 5 sq +
//
// For Defined Functions Source them from the heap than return the instruction pointer to its original pos
//
// 3 [ dup * ] I = 9 
