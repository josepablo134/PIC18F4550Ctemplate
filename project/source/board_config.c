#include "../../include/config/board_config.h"
#include "../../include/bldrCtl/bldrCtl_flashWrapper.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    BLDR_flashISR();
}

void __interrupt(low_priority) Board_LP_ISR(){
    
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