/*
 * @file usInternal.h
 *
 * @brief Microservice Internal Definitions
 *
 ******************************************************************************/

#ifndef __US_INTERNAL_H
#define __US_INTERNAL_H

/********************************* INCLUDES ***********************************/

#include "uService.h"

/***************************** MACRO DEFINITIONS ******************************/

/***************************** TYPE DEFINITIONS *******************************/

typedef enum
{
    /* List of Operations */
} usOperations;

typedef struct
{
    /* List of parameters */
} usPayloadOpenSession;

typedef struct
{
    /* List of parameters */
} usPayloadCloseSession;

typedef struct
{
    uServicePackageHeader header;

    struct
    {

    }flags;
}  usPayloadOperationA;

typedef struct
{
    uServicePackageHeader header;

    struct
    {

    }flags;
}  usPayloadOperationB;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

#endif /* __US_INTERNAL_H */
