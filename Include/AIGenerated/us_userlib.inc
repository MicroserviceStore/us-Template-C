SysStatus us_Template_Sum(int32_t a, int32_t b, int32_t* result, usStatus* usStatus)
{
    const uint32_t timeoutInMs = 2000;
    SysStatus retVal;

    usResponsePackage response;
    usRequestPackage request;

    {
        request.header.operation = usOp_Sum;
        request.header.length = sizeof(request);
        request.payload.sum.a = a;
        request.payload.sum.b = b;
    };

    retVal = uService_RequestBlocker(userLibSettings.execIndex, (uServicePackage*)&request, (uServicePackage*)&response, timeoutInMs);
    *usStatus = response.header.status;

    if (*usStatus == usStatus_Success)
    {
        *result = response.payload.sum.result;
    }

    return retVal;
}
