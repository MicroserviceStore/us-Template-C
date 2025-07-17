
#define LOG_INFO_ENABLED 1
#define LOG_WARNING_ENABLED 1
#define LOG_ERROR_ENABLED 1
#include "SysCall.h"

#include "us-Template.h"

#define CHECK_TEMPLATE_ERR(sysStatus, usStatus) \
                if (sysStatus != SysStatus_Success || usStatus != usTemplate_Success) \
                { LOG_PRINTF(" > us Test Failed. Line %d. Sys Status %d | us Status %d", __LINE__, sysStatus, usStatus); return; }
int main(void)
{
    SysStatus retVal;

    LOG_PRINTF(" > Container : Microservice Test User App");

    SYS_INITIALISE_IPC_MESSAGEBOX(retVal, 4);

    retVal = us_Template_Initialise();
    CHECK_TEMPLATE_ERR(retVal, 0);

    {
        usTemplateStatus usStatus;
        int32_t a = 5;
        int32_t b = 6;
        int32_t expectedResult = a + b;
        int32_t result = 0;

        retVal = us_Template_Sum(a, b, &result, &usStatus);
        CHECK_TEMPLATE_ERR(retVal, usStatus);

        LOG_PRINTF(" > us Test %s", result == expectedResult ? "Success" : "Failed");
    }
}
