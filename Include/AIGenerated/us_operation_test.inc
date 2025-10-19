{
    usStatus status;
    int32_t a = 5;
    int32_t b = 6;
    int32_t expectedResult = a + b;
    int32_t result = 0;

    retVal = us_Template_Sum(a, b, &result, &status);
    CHECK_US_ERR(retVal, status);

    LOG_PRINTF(" > us Sum Test %s", result == expectedResult ? "Success" : "Failed");
}
