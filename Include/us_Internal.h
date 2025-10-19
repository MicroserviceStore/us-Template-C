/*
 * @file us_Internal.h
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
    /*
     * List of Operations
     *  - AI Generated ("us_operations.inc" below)
     *  - or, Manually Add below
     */
    #include "us_operations.inc"
} usOperations;

typedef struct
{
    uServicePackageHeader header;

    union
    {
        /*
        * List of Inputs of Each Operation defined in usOperations
        *  - AI Generated ("us_operation_inputs.inc" below)
        *  - or, Manually Add below
        */
        #include "us_operation_inputs.inc"
    } payload;
} usRequestPackage;

typedef struct
{
    uServicePackageHeader header;

    union
    {
        /*
        * List of Outputs of Each Operation defined in usOperations
        *  - AI Generated ("us_operation_outputs.inc" below)
        *  - or, Manually Add below
        */
        #include "us_operation_outputs.inc"
    } payload;
} usResponsePackage;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

#endif /* __US_INTERNAL_H */
