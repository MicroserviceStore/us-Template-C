
/* Enable all the log levels */
#define LOG_INFO_ENABLED        1
#define LOG_WARNING_ENABLED     1
#define LOG_ERROR_ENABLED       1
#include "SysCall.h"

#include "us_public_headers.inc"

#define CHECK_US_ERR(_sysStatus, _usStatus) \
                if (_sysStatus != SysStatus_Success || _usStatus != usStatus_Success) \
                { LOG_PRINTF(" > us Test Failed. Line %d. Sys Status %d | us Status %d. Exiting the User Application...", __LINE__, _sysStatus, _usStatus); Sys_Exit(); }

int main(void)
{
    SysStatus retVal;

    LOG_PRINTF(" > Container : Microservice Test User App");

    SYS_INITIALISE_IPC_MESSAGEBOX(retVal, 4);

    #include "us_init_func.inc"

    #include "us_operation_test.inc"

    LOG_PRINTF(" > Exiting the User Application");
    /* Exit the Container */
    Sys_Exit();

    return 0;
}
