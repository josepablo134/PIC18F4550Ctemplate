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

    #define DISABLE_GEI()    { asm("BCF INTCON,7"); }
    #define DISABLE_GEIH()   { asm("BCF INTCON,7"); }
    #define DISABLE_GEIL()   { asm("BCF INTCON,6"); }

    #define ENABLE_GEI()    { asm("BSF INTCON,7"); }
    #define ENABLE_GEIH()   { asm("BSF INTCON,7"); }
    #define ENABLE_GEIL()   { asm("BSF INTCON,6"); }

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif
