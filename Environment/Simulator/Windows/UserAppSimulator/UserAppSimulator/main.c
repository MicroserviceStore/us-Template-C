
/* Enable all the log levels */
#define LOG_INFO_ENABLED        1
#define LOG_WARNING_ENABLED     1
#define LOG_ERROR_ENABLED       1
#include "SysCall.h"

#ifdef US_AI_GENERATED
    #include "us_public_headers.inc"
#else
    #include "us-Template.h"
#endif /* US_AI_GENERATED */

#define CHECK_US_ERR(_sysStatus, _usStatus) \
                if (_sysStatus != SysStatus_Success || _usStatus != usStatus_Success) \
                { LOG_PRINTF(" > us Test Failed. Line %d. Sys Status %d | us Status %d. Exiting the User Application...", __LINE__, _sysStatus, _usStatus); Sys_Exit(); }

int main(void)
{
    SysStatus retVal;

    LOG_PRINTF(" > Container : Microservice Test User App");

    SYS_INITIALISE_IPC_MESSAGEBOX(retVal, 4);

#ifdef US_AI_GENERATED
    #include "us_init_func.inc"
    #include "us_operation_test.inc"
#else /* US_AI_GENERATED */
    us_Template_Initialise();

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
#endif /* US_AI_GENERATED */

    LOG_PRINTF(" > Exiting the User Application");
    /* Exit the Container */
    Sys_Exit();

    return 0;
}
