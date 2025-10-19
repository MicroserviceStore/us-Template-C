case usOp_Sum:
    response.payload.sum.result = request->payload.sum.a + request->payload.sum.b;
    response.header.status = usStatus_Success;
    response.header.length = sizeof(response.payload.sum);
    retVal = Sys_SendMessage(senderID, (uint8_t*)&response, sizeof(usResponsePackage), &sequenceNo);
    break;
