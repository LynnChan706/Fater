
#include "OS_KMLGCCCompiler.h"

#include <linux/module.h>

#if !( defined( _PT_64_ ) || defined( __LP64__ ) )

#include "asm/div64.h"

OS_UINT64 OS_DIV64( OS_UINT64 a, OS_UINT64 b )
{
    OS_UINT64 lReturn = a;
    do_div( lReturn, b );
    return lReturn;
}

OS_UINT64 OS_MOD64( OS_UINT64 a, OS_UINT64 b )
{
    OS_UINT64 lReturn = a;
    return do_div( lReturn, b );
} 
 
OS_VOID OS_Prink( const char* aFormat, ... )
{
	va_list lArgs;

    va_start( lArgs, aFormat );
	vprintk( aFormat, lArgs );
    va_end( lArgs );
}
#endif 


