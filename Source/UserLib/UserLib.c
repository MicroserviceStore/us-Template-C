/*
 * @file
 *
 * @brief Microservice API static library source file. This source file/library
 *        runs in the caller execution.
 *
 ******************************************************************************/

/********************************* INCLUDES ***********************************/

#include USERVICE_PUBLIC_HEADER
#include USERVICE_INTERNAL_HEADER

#include "uService.h"

/***************************** MACRO DEFINITIONS ******************************/

#define US_OPTIONAL     0

/***************************** TYPE DEFINITIONS *******************************/
typedef struct
{
    struct
    {
        //uint32_t initialised        : 1;
    } flags;

    /*
     * The "Execution Index" is a system wide enumaration by the Microservice Runtime
     * to interact with the Microservice.
     */
    uint32_t execIndex;
} uS_UserLibSettings;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/
PRIVATE uS_UserLibSettings userLibSettings;

PRIVATE const char usName[SYS_EXEC_NAME_MAX_LENGTH] = USERVICE_NAME;

/***************************** PRIVATE FUNCTIONS *******************************/

/***************************** PUBLIC FUNCTIONS *******************************/
#define INITIALISE_FUNCTIONEXPAND(a, b, c) a##b##c
#define INITIALISE_FUNCTION(name) INITIALISE_FUNCTIONEXPAND(us_, name, _Initialise)
SysStatus INITIALISE_FUNCTION(USERVICE_NAME_NONSTR)(void)
{
    /* Get the Microservice Index to interact with the Microservice */
    return uService_Initialise(usName, &userLibSettings.execIndex);
}

#if 0
SysStatus us_tinyAES_OpenSession(/* Custom Arg List to open a session */,
                                 /* Out uService_Status*/)
{
    SysStatus retVal;
    usRequestPackage request;
    usResponsePackage response;

    {
        request.header.operation = usOp_OpenSession;
        request.header.length = <SET>;
        request.payload = <SET>;
    }

    retVal = uService_RequestBlocker(tinyAESSettings.execIndex, (uServicePackage*)&request, (uServicePackage*)&response, timeoutInMs);
    
    if (retVal == SysStatus_Success)
    {
        /* Interaction successfull but Microservice may have custom status reporting */
        *status = response.header.status;

        /* Set all output parameters using response.payload */
    }

    return retVal;
}

SysStatus us_tinyAES_CloseSession(/* Custom Arg List to open a session */,
                                  /* Out uService_Status*/)
{
    SysStatus retVal;
    usRequestPackage request;
    usResponsePackage response;

    {
        request.header.operation = usOp_CloseSession;
        request.header.length = <SET>;
        request.payload = <SET>;
    }

    retVal = uService_RequestBlocker(tinyAESSettings.execIndex, (uServicePackage*)&request, (uServicePackage*)&response, timeoutInMs);
    
    if (retVal == SysStatus_Success)
    {
        /* Interaction successfull but Microservice may have custom status reporting */
        *status = response.header.status;

        /* Set all output parameters using response.payload */
    }

    return retVal;
}

#endif
