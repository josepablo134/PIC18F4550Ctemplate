#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include "project_config.h"

    #ifdef __DEBUG
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

    #define SET_PC( ADDR )  {\
        PCLATU = 0;\
        PCLATH = ADDR >> 8U;\
        PCLAT  = 0xFF & ADDR;\
    }

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

