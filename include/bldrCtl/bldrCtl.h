#ifndef _BLDR_CTL_H_
#define	_BLDR_CTL_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include <stdint.h>

    #include "bldrCtl_cfg.h"
    #include "bldrCtl_types.h"
    
    extern void BLDR_init();
    
    extern bldr_state BLDR_getState();
    
    extern bldr_state BLDR_MainHandler();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

