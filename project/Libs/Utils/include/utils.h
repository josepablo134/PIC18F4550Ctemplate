#ifndef _UTILS_H_
#define	_UTILS_H_

#include "utils_include.h"
#include "utils_types.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

extern const char int2hex_ascii[ 16U ];
/**
 * @brief map vector v1 to v2.
 * 
 * Since this is a macro, the type is defined in the arguments.
 * For example:
 * @code{.c}
 *      map( x , (int32_t)x1, (int32_t)x2, (int32_t)y1, (int32_t)y2 )
 * @endcode
*/
#define map( x, x1, x2, y1, y2 )    ( ( (x-x1)*(y2-y1) / (x2-x1) ) + y1 )

#ifdef	__cplusplus
}
#endif

#endif
