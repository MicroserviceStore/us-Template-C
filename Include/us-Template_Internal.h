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
    usTemplateOp_Sum
    /* List of Operations */
} usTemplateOperations;

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
} usTemplateRequestPackage;

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
} usTemplateResponsePackage;

/**************************** FUNCTION PROTOTYPES *****************************/

/******************************** VARIABLES ***********************************/

#endif /* __US_INTERNAL_H */
