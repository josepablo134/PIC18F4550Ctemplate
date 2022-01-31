#include "../../inc/Board/board_config.h"
#include "../../inc/project_macros.h"
#include "../../inc/bldrCtl/bldrCtl_cfg.h"
#include "../../inc/Board/UART/UART.h"

#define T1_SCK_FREQ             32768U
#define T1_SCK_DIV              1U
#define T1_FREQ                 (T1_SCK_FREQ/T1_SCK_DIV)
#define T1_MAX_VAL              65536U
#define T1_1MS                  (T1_FREQ/1000U)
#define T1_SYSTICK_PRECHARGE    ((uint16_t)T1_MAX_VAL - T1_1MS)

#define TMR1_ISR()              {\
    if( PIR1bits.TMR1IF ){\
        TMR1 += ((uint16_t)T1_SYSTICK_PRECHARGE);\
        PIR1bits.TMR1IF = 0;\
    }\
}

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