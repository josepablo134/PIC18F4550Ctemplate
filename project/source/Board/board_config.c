#include "../../inc/Board/board_config.h"
#include "../../inc/Board/UART/UART_internal.h"
#include "../../inc/Board/Timer1/Timer1_internal.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    if( PIR1bits.TMR1IF ){
        LATAbits.LA3 = !PORTAbits.RA3;
        TMR1_ISR();
    }
}

void __interrupt(low_priority) Board_LP_ISR(void){
    UART_ISR();
}

void Board_Init(){
    /**
     * Enable PortA bit 4
     */
    ADCON1 = 0x0F;
    CMCON = 0x07;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    LATAbits.LA2 = 0;
    LATAbits.LA3 = 0;
    LATAbits.LA4 = 0;
    
    /**
     * Enable Int. Priority
     */
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}