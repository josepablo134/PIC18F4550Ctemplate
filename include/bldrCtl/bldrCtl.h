#ifndef _BLDR_CTL_H_
#define	_BLDR_CTL_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include "bldrCtl_cfg.h"
    #include "bldrCtl_types.h"
    
    extern void BLDR_Init(void);
    
    extern void BLDR_tearDown(void);
    
    extern BldrState_t BLDR_getState(void);
    
    extern BldrState_t BLDR_MainHandler(void);
    
    extern BldrStdReturn_t BLDR_PassControl( Bldr_Appl_Addr_t addr );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

