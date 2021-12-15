#include "../../../include/bldrCtl/bldrCtl_flashWrapper.h"
#include "../../../include/config/board_config.h"

void BLDR_flashInit(){
    PIE2bits.EEIE = 0;//Flash Int Enable
    PIR2bits.EEIF = 0;//Flash Int. Flag
    IPR2bits.EEIP = 1;//Flash Int. High Prior.
    PIE2bits.EEIE = 1;//Flash Int Enable
}

uint8_t BLDR_flashErase( const bldr_address address,
        bldr_size size,
        const bldr_byte data ){
    return 0;
}

uint8_t BLDR_flashWriteBuffer( const bldr_address address,
        const bldr_size size,
        const bldr_byte *buffer ){
    return 0;
}

inline void BLDR_flashISR(){
    if( PIR2bits.EEIF ){
        PIR2bits.EEIF = 0;
        /// Do something
    }
}