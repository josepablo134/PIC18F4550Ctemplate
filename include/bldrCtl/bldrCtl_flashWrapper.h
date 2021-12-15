#ifndef _BLDR_CTL_FLS_WRAPPER_H_
#define	_BLDR_CTL_FLS_WRAPPER_H_

#ifdef	__cplusplus
extern "C" {
#endif

    #include "bldrCtl_types.h"
    
    extern void BLDR_flashInit();
    
    extern uint8_t BLDR_flashErase( const bldr_address address,
            bldr_size size,
            const bldr_byte data );
    
    extern uint8_t BLDR_flashWriteBuffer( const bldr_address address,
            const bldr_size size,
            const bldr_byte *buffer );
    
    extern inline void BLDR_flashISR();

#ifdef	__cplusplus
}
#endif

#endif