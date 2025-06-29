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
    usOp_Sum
    /* List of Operations */
} usOperations;

typedef struct
{
    uServicePackageHeader header;

    union
    {
        struct
        {
            int32_t a;
            int32_t b;
        } sum;
    } payload;
} usRequestPackage;

typedef struct
{
    uServicePackageHeader header;

    union
    {
        struct
        {
            int32_t result;
        } sum;
    } payload;
} usResponsePackage;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

#endif /* __US_INTERNAL_H */
