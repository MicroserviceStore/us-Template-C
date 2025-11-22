#ifdef SP_LINT
#undef DEBUG_LEVEL
#define DEBUG_LEVEL                 0
#endif

#include "operation_func.inc"

/* Do not include the below for Splint */
#ifndef SP_LINT

#include "unity.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Constructor Method for each test case
 *
 */
void setUp(void)
{
}

/**
 * @brief Destructor Method for each test case
 *
 */
void tearDown(void)
{
}

void test_HelloWorld(void)
{
    TEST_ASSERT(true);
}

void test_sum(void)
{
    int a = 3;
    int b = 4;
    int result = 0;
    int expected = a + b;

    result = sum(a, b);
    TEST_ASSERT_EQUAL(expected, result);
}

#endif /* SP_LINT */
