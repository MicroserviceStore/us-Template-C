#include "SysCall.h"

PRIVATE void startService(void);
PRIVATE SysStatus processRequest(uint8_t senderID, usRequestPackage* request);

int main()
{
    SysStatus retVal;

    LOG_PRINTF("uS Template");

    SYS_INITIALISE_IPC_MESSAGEBOX(retVal, <CFG_MAX_NUM_OF_SESSION>);
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
    while (true)
    {
        bool dataReceived = false;
        int receivedLen = 0;
        
        /* Sleep until receive an IPC message */
        Sys_WaitForEvent(SysEvent_IPCMessage);

        (void)Sys_IsMessageReceived(&dataReceived, &receivedLen, &sequenceNo);       
        if (!dataReceived || receivedLen == 0)
        {
            continue;
        }

        /*
         * TODO : Check for "receivedLen" if enough or will exceed the receive buffer?
         */

        /* Get the message */
        (void)Sys_ReceiveMessage(&senderID, (uint8_t*)&request, receivedLen, &sequenceNo);

        /* Process the request */
        processRequest(senderID, &request);
    }
}

PRIVATE SysStatus processRequest(uint8_t senderID, usRequestPackage* request)
{
    switch (request->header.operation)
    {
        case /**/:
            /* 
             * 1. Process the payload
             * 2. Prepare Request
             * 3. Send response to the Sender using senderID
             */             
            Sys_SendMessage(senderID, (uint8_t*)&response, sizeof(usResponsePackage), &sequenceNo);
            break;
        default:
            break;
    }
}
