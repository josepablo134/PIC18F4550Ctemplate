#include "../../include/config/board_config.h"
#include "../../include/config/project_macros.h"
#include "../../include/bldrCtl/bldrCtl_cfg.h"
#include "../../include/UART/UART.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    // SET_PC( BLDR_CFG_APPL_HIGH_PRIOR_ISR );
}

void __interrupt(low_priority) Board_LP_ISR(void){
    // SET_PC( BLDR_CFG_APPL_LOW_PRIOR_ISR );
    UART_INTERRUPT;
}

void Board_Init(){
    /**
     * Enable PortA bit 4
     */
    ADCON1 = 0x0F;
    CMCON = 0x07;
    TRISAbits.RA4 = 0;
    LATAbits.LA4 = 0;
    
    /**
     * Enable Int. Priority
     */
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}