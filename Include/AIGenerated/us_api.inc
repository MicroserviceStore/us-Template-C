/*
 * Initialise the Microservice
 *
 * @param none
 *
 * @retval SysStatus_Success Success
 * @retval SysStatus_NotFound The Microservice not found on the device.
 */
SysStatus us_Template_Initialise(void);

/*
 * A simple sum operation
 *
 * @param a Input A
 * @param b Input B
 * @param[out] result A+B
 * @param[out] usStatus uService Specific Status
 *
 * @retval SysStatus_Success Success
 * @retval SysStatus_NotFound The Microservice not found on the device.
 */
SysStatus us_Template_Sum(int32_t a, int32_t b, int32_t* result, usStatus* usStatus);
