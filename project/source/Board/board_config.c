#include "../../inc/Board/board_config.h"
#include "../../inc/Board/Mcu/Mcu.h"
#include "../../inc/Board/UART/UART_internal.h"
#include "../../inc/Board/Timer1/Timer1_internal.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    if( PIR1bits.TMR1IF ){
        TMR1_ISR();
    }
    if( INTCONbits.RBIF ){
        LATAbits.LA4 = !PORTAbits.RA4;
        //INTCONbits.RBIE = 0U;
        PORTB = 0x00;
        INTCONbits.RBIF = 0U;
    }
}

void __interrupt(low_priority) Board_LP_ISR(void){
    UART_ISR();
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
    
    /**
     * Enable PortA bit 4
     */
    ADCON1 = 0x0FU;
    CMCON = 0x07U;
    TRISAbits.RA2 = 0U;
    TRISAbits.RA3 = 0U;
    TRISAbits.RA4 = 0U;
    LATAbits.LA2 = 0U;
    LATAbits.LA3 = 0U;
    LATAbits.LA4 = 0U;
    
    /**
     * Enable Int. Priority
     */
    RCONbits.IPEN = 1U;
    INTCONbits.GIEH = 1U;
    INTCONbits.GIEL = 1U;
}