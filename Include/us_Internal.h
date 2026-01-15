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
#ifdef US_AI_GENERATED    
    #include "us_operations.inc"
#else /* US_AI_GENERATED */
    usOp_Sum
#endif /* US_AI_GENERATED */
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
#ifdef US_AI_GENERATED
        #include "us_operation_inputs.inc"
#else /* US_AI_GENERATED */
    struct
    {
        int32_t a;
        int32_t b;
    } sum;
#endif /* US_AI_GENERATED */
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
#ifdef US_AI_GENERATED
        #include "us_operation_outputs.inc"
#else /* US_AI_GENERATED */
    struct
    {
        int32_t result;
    } sum;
#endif /* US_AI_GENERATED */
    } payload;
} usResponsePackage;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

#endif /* __US_INTERNAL_H */
