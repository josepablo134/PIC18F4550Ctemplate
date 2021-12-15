#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include "project_config.h"

    #ifdef DEBUG

    extern void __error__(void);

    #define ASSERT(expr) do                                                       \
                         {                                                        \
                             if(!(expr))                                          \
                             {                                                    \
                                 __error__();                                     \
                             }                                                    \
                         }                                                        \
                         while(0)
    #else
    #define ASSERT(expr)
    #endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
