/*
 * OS_UMLString.c
 *
 *  Created on: Jul 13, 2010
 *      Author: fgobeil
 */

#include "OS_KMLString.h"

#include <linux/kernel.h>

OS_INT32 OS_StringFormat( OS_CHAR* aLine, OS_SIZE aLineSize, const OS_CHAR* aFormat, ... )
{
    OS_INT32 lReturn;
    va_list lArgs;

    va_start( lArgs, aFormat );
    lReturn = vsnprintf( aLine, aLineSize, aFormat, lArgs );
    va_end( lArgs );

    return lReturn;
}
