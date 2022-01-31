#ifndef _PROJECT_MACROS_H_
#define	_PROJECT_MACROS_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #ifdef __DEBUG
    extern void __error__(void);

    #define ASSERT(expr) do                         \
                         {                          \
                             if(!(expr))            \
                             {                      \
                                 __error__();       \
                             }                      \
                         }                          \
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

