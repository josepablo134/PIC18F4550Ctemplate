#include "../../../include/config/board_config.h"
#include "../../../include/bldrCtl/bldrCtl.h"
#include "../../../include/config/project_macros.h"

static BldrState_t __state = BLDR_UNINIT;

void BLDR_Init(){
    Board_Init();
}

void BLDR_tearDown(){
}

BldrState_t BLDR_getState(){
    return __state;
}

BldrState_t BLDR_MainHandler(){
    ASSERT( BLDR_NOT_OK != BLDR_PassControl( BLDR_CFG_APPL_START ) );
    return __state;
}

BldrStdReturn_t BLDR_PassControl( Bldr_Appl_Addr_t addr ){
    /**
     * Disable interrupts and peripherals used by the bootloader
     */
    BLDR_tearDown();
    STKPTR = 0;
    SET_PC( addr );
    /*This is not possible!*/
    return BLDR_NOT_OK;
}