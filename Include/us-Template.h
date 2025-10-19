/*
 * @file us.h
 *
 * @brief Microservice Public API
 *
 ******************************************************************************/

#ifndef __US_H
#define __US_H

/********************************* INCLUDES ***********************************/

#include "uService.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

/*
 * Default Status
 */
typedef enum
{
    usStatus_Success = 0,
    /* Operation not defined or the access not granted */
    usStatus_InvalidOperation,
    /* Timeout occurred during the opereration */
    usStatus_Timeout,
    /* Microservice does not have any available session */
    usStatus_NoSessionSlotAvailable,
    /* Request to an invalid session */
    usStatus_InvalidSession,
    /* Invalid Parameter - Insufficient Input or Output Size  */
    usStatus_InvalidParam_UnsufficientSize,
    /* Invalid Parameter - Input or Output exceeds the allowed capacity  */
    usStatus_InvalidParam_SizeExceedAllowed,

    /* The developer can defines custom statuses */
    usStatus_CustomStart = 32
} usStatus;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

/*******************************************************************************
 * Microservice Public API
 *  - AI Generated ("us-api.inc" below)
 *  - or, Manually Add the API below
 ******************************************************************************/
#include "us_api.inc"

#endif /* __US_H */
