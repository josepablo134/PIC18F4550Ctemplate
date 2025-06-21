#include "board_config.h"
#include "Mcu.h"
#include "UART.h"
#include "UART_internal.h"

void __interrupt(high_priority) Board_HP_ISR(void){
}

void __interrupt(low_priority) Board_LP_ISR(void){
    UART_ISR();
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
    /** enable int priority + enable both priority interrupts */
    RCONbits.IPEN = 1U;
    INTCONbits.GIEH = 1U;
    INTCONbits.GIEL = 1U;

    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;
}