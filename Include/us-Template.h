/*
 * @file usInternal.h
 *
 * @brief Microservice Internal Definitions
 *
 ******************************************************************************/

#ifndef __US_H
#define __US_H

/********************************* INCLUDES ***********************************/

#include "uService.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

typedef enum
{
    usTemplate_Success = 0,
    usTemplate_InvalidOperation,
    usTemplate_NoSessionSlotAvailable,
    usTemplate_NoSession,
    usTemplate_InvalidSession,

    usTemplate_Timeout,

    usTemplate_UnsupportedOperation,

    usTemplate_InvalidParam_UnsufficientSize,
    usTemplate_InvalidParam_SizeExceedAllowed,

    usTemplate_InvalidParam_Key,
} usTemplateStatus;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

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
SysStatus us_Template_Sum(int32_t a, int32_t b, int32_t* result, usTemplateStatus* usStatus);

#endif /* __US_H */
