#include "board_config.h"
#include "Mcu.h"
#include "UART.h"

void __interrupt(high_priority) Board_HP_ISR(void){
}

void __interrupt(low_priority) Board_LP_ISR(void){
    UART_TX_ISR();
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
    /** enable int priority */
    RCONbits.IPEN = 1U;
    /** enable all interrupts */
    INTCONbits.GIEH = 1U;
    INTCONbits.GIEL = 1U;
}