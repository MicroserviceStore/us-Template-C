#include "uService.h"

#include "us-Template.h"
#include "us-Template_Internal.h"

#ifndef CFG_US_MAX_NUM_OF_SESSION

#define CFG_US_MAX_NUM_OF_SESSION   1       /* Let us allow one session at a time */
#endif

PRIVATE void startService(void);
PRIVATE SysStatus processRequest(uint8_t senderID, usTemplateRequestPackage* request);
PRIVATE void sendError(uint8_t receiverID, uint16_t operation, uint8_t status);

int main()
{
    SysStatus retVal;

    LOG_PRINTF("uS Template");

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
    usTemplateRequestPackage request;

    uint32_t sequenceNo;
    (void)sequenceNo;
    usTemplateStatus responseStatus;
    uint8_t senderID = 0xFF;

    while (true)
    {
        bool dataReceived = false;
        int receivedLen = 0;
        responseStatus = usTemplate_Success;

        (void)Sys_IsMessageReceived(&dataReceived, &receivedLen, &sequenceNo);
        if (!dataReceived || receivedLen == 0)
        {
            /* Sleep until receive an IPC message */
            Sys_WaitForEvent(SysEvent_IPCMessage);

            continue;
        }

        if (receivedLen <= USERVICE_PACKAGE_HEADER_SIZE)
        {
            responseStatus = usTemplate_InvalidParam_UnsufficientSize;
            LOG_PRINTF(" > Unsufficint Mandatory Received Length (%d)/(%d)",
                receivedLen, USERVICE_PACKAGE_HEADER_SIZE);
        }

#if 0
        if (responseStatus == usTemplate_Success && 
            receivedLen > <PACKAGE_MAX_SIZE>)
        {
            responseStatus = usTemplate_InvalidParam_SizeExceedAllowed;

            LOG_PRINTF(" > Received Length (%d) exceed than allowed length(%d)",
                receivedLen, <PACKAGE_MAX_SIZE>);

            /* Let us just get the header, as not need for the payload */
            receivedLen = USERVICE_PACKAGE_HEADER_SIZE;
        }
#endif

        /* Get the message */
        (void)Sys_ReceiveMessage(&senderID, (uint8_t*)&request, receivedLen, &sequenceNo);

        /* Do not process the message if there was an error */
        if (responseStatus != usTemplate_Success)
        {
            sendError(senderID, request.header.operation, responseStatus);
            continue;
        }

        /* Process the request */
        processRequest(senderID, &request);
    }
}

PRIVATE SysStatus processRequest(uint8_t senderID, usTemplateRequestPackage* request)
{
    SysStatus retVal = SysStatus_Success;
    usTemplateResponsePackage response;
    uint32_t sequenceNo;

    response.header = request->header;

    switch (request->header.operation)
    {
        case usTemplateOp_Sum:
            response.payload.sum.result = request->payload.sum.a + request->payload.sum.b;
            response.header.status = usTemplate_Success;
            response.header.length = sizeof(response.payload.sum);

;           retVal = Sys_SendMessage(senderID, (uint8_t*)&response, sizeof(usTemplateResponsePackage), &sequenceNo);
            break;
        default:
            sendError(senderID, response.header.operation, usTemplate_UnsupportedOperation);
            break;
    }

    return retVal;
}

PRIVATE void sendError(uint8_t receiverID, uint16_t operation, uint8_t status)
{
    uint32_t sequenceNo;
    (void)sequenceNo;
    usTemplateResponsePackage response =
    {
        .header.operation = operation,
        .header.status = status,
        .header.length = 0
    };

    (void)Sys_SendMessage(receiverID, (uint8_t*)&response, sizeof(response), &sequenceNo);
}
