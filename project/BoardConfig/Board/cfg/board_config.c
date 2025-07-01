#include "board_config.h"
#include "Mcu.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    /** Pass control to App HISR */
    asm("GOTO 0x002008");
}

void __interrupt(low_priority) Board_LP_ISR(void){
    /** Pass control to App LISR */
    asm("GOTO 0x002018");
}

/**
 * Dummy User Application
*/
void __at(0x2000) dummy_app_rst(void);

void __at(0x2020) dummy_app_main(void);

void dummy_app_rst( void ) {
    asm("GOTO 0x2020");
}

void dummy_app_main( void ) {
    OSCCON = 0x00;
    OSCTUNE = 0x00;
    OSCCONbits.IRCF = MCU_CFG_INT_OSC;
    OSCCONbits.SCS = MCU_CFG_DEFAULT_OSC;
    OSCTUNEbits.INTSRC = MCU_CFG_LP_CLOCK;
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

    while( 1U ){
        /// Allows execution time measurement through an oscilloscope
        asm("BTG  LATA,4");
        __delay_ms( 500 );
    }
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
    /** enable int priority */
    RCONbits.IPEN = 1U;
    /** disable all interrupts */
    INTCONbits.GIEH = 0U;
    INTCONbits.GIEL = 0U;
}