#ifndef _BLDR_CTL_CFG_H_
#define	_BLDR_CTL_CFG_H_

    #include "../Board/flashCtl/flash_cfg.h"
    #include "bldrCtl_types.h"

    #define BLDR_CFG_LOCAL_BUFFER_SIZE      FLASH_CFG_SECTOR_SIZE
    #define BLDR_CFG_APPL_START             (0x800U)
    #define BLDR_CFG_APPL_HIGH_PRIOR_ISR    (0x808U)
    #define BLDR_CFG_APPL_LOW_PRIOR_ISR     (0x818U)

#endif	/* XC_HEADER_TEMPLATE_H */