#define LOG_ERROR_ENABLED   1
#define LOG_WARNING_ENABLED 1
#define LOG_INFO_ENABLED    1
#include "uService.h"

#ifdef US_AI_GENERATED
    #include "us_public_headers.inc"
#else /* US_AI_GENERATED */
    #include "us-Template.h"
#endif /* US_AI_GENERATED */

#include "us_Internal.h"

#ifndef CFG_US_MAX_NUM_OF_SESSION
#define CFG_US_MAX_NUM_OF_SESSION   1       /* Let us allow one session at a time */
#endif

#ifdef US_AI_GENERATED
    #include "operation_func.inc"
#else /* US_AI_GENERATED */
/* Test Sum function for the template */
int sum(int a, int b) { return a + b; }

#endif /* US_AI_GENERATED */

PRIVATE void startService(void);
PRIVATE void processRequest(uint8_t senderID, usRequestPackage* request);
PRIVATE void sendError(uint8_t receiverID, uint16_t operation, uint8_t status);

int main()
{
    SysStatus retVal;

    uService_PrintIntro();

    SYS_INITIALISE_IPC_MESSAGEBOX(retVal, CFG_US_MAX_NUM_OF_SESSION);
    if (retVal != SysStatus_Success)
    {
        LOG_ERROR("Failed to initialise MessageBox. Error : %d", retVal);
    }
    else
    {
        startService();
    }

    LOG_ERROR("Exiting the Microservice...");
    Sys_Exit();
}

PRIVATE void startService(void)
{
    usRequestPackage request;

    uint32_t sequenceNo;
    (void)sequenceNo;
    usStatus responseStatus;
    uint8_t senderID = 0xFF;

    while (true)
    {
        bool dataReceived = false;
        uint32_t receivedLen = 0;
        responseStatus = usStatus_Success;

        (void)Sys_IsMessageReceived(&dataReceived, &receivedLen, &sequenceNo);
        if (!dataReceived || receivedLen == 0)
        {
            /* Sleep until receive an IPC message */
            Sys_WaitForEvent(SysEvent_IPCMessage);

            continue;
        }

        if (receivedLen <= USERVICE_PACKAGE_HEADER_SIZE)
        {
            responseStatus = usStatus_InvalidParam_UnsufficientSize;
            LOG_PRINTF(" > Unsufficint Mandatory Received Length (%d)/(%d)",
                receivedLen, USERVICE_PACKAGE_HEADER_SIZE);
        }

#if 0
        if (responseStatus == usStatus_Success && 
            receivedLen > <PACKAGE_MAX_SIZE>)
        {
            responseStatus = usStatus_InvalidParam_SizeExceedAllowed;

            LOG_PRINTF(" > Received Length (%d) exceed than allowed length(%d)",
                receivedLen, <PACKAGE_MAX_SIZE>);

            /* Let us just get the header, as not need for the payload */
            receivedLen = USERVICE_PACKAGE_HEADER_SIZE;
        }
#endif

        /* Get the message */
        (void)Sys_ReceiveMessage(&senderID, (uint8_t*)&request, receivedLen, &sequenceNo);

        /* Do not process the message if there was an error */
        if (responseStatus != usStatus_Success)
        {
            sendError(senderID, request.header.operation, responseStatus);
            continue;
        }

        /* Process the request */
        processRequest(senderID, &request);
    }
}

PRIVATE void processRequest(uint8_t senderID, usRequestPackage* request)
{
    SysStatus retVal = SysStatus_Success;
    usResponsePackage response;
    uint32_t sequenceNo;

    response.header = request->header;

    switch (request->header.operation)
    {
        /*
         * Request Parser of Each Operation defined in usOperations
         *  - AI Generated ("us_operation_parser.inc" below)
         *  - or, Manually Add Cases for each operation below
         */
#ifdef US_AI_GENERATED
        #include "us_operation_parser.inc"
#else /* US_AI_GENERATED */
        case usOp_Sum:
            response.payload.sum.result = sum(request->payload.sum.a, request->payload.sum.b);
            response.header.status = usStatus_Success;
            response.header.length = sizeof(response.payload.sum);
            retVal = Sys_SendMessage(senderID, (uint8_t*)&response, sizeof(usResponsePackage), &sequenceNo);
            break;
#endif /* US_AI_GENERATED */

        /* Unrecognised operation */
        default:
            sendError(senderID, response.header.operation, usStatus_InvalidOperation);
            break;
    }

    return retVal;
}

PRIVATE void sendError(uint8_t receiverID, uint16_t operation, uint8_t status)
{
    uint32_t sequenceNo;
    (void)sequenceNo;
    usResponsePackage response =
    {
        .header.operation = operation,
        .header.status = status,
        .header.length = 0
    };

    (void)Sys_SendMessage(receiverID, (uint8_t*)&response, sizeof(response), &sequenceNo);
}
