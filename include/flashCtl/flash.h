#ifndef _FLS_CTL_H_
#define	_FLS_CTL_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * EXTERNAL RESOURCES
 */
    
#include "flash_cfg.h"
#include "flash_types.h"

/**
 * 
 * FUNCTION DECLARATION
 */
    
extern FlashStdReturn_t FlashErase( FlashAddress_t addr , FlashSize_t length );

extern FlashStdReturn_t FlashRead( FlashAddress_t addr, FlashSize_t length , uint8_t* data );

extern FlashStdReturn_t FlashWrite( FlashAddress_t addr, FlashSize_t length , const uint8_t* data );

extern FlashStdReturn_t FlashValidate( FlashAddress_t addr , FlashSize_t length , const uint8_t* data );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
