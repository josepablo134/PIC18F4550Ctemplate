#include "board_config.h"
#include "Mcu.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    /** Pass control to App HISR */
    asm("GOTO 0x000808");
}

void __interrupt(low_priority) Board_LP_ISR(void){
    /** Pass control to App LISR */
    asm("GOTO 0x000818");
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
    /** enable int priority */
    RCONbits.IPEN = 1U;
    /** disable all interrupts */
    INTCONbits.GIEH = 0U;
    INTCONbits.GIEL = 0U;

    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;
}