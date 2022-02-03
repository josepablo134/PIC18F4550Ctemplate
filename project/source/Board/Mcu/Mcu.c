#include "../../../inc/Board/Mcu/Mcu.h"

void Mcu_Init(void){
    /* Nothing to do */
}

void Mcu_Open(void){
    OSCCON = 0x00;
    OSCTUNE = 0x00;
    OSCCONbits.IRCF = MCU_CFG_INT_OSC;
    OSCCONbits.SCS = MCU_CFG_DEFAULT_OSC;
    OSCTUNEbits.INTSRC = MCU_CFG_LP_CLOCK;
}

void Mcu_Reset(void){
    asm("RESET");
}

void Mcu_Sleep(){
    OSCCONbits.IDLEN = 1U;
    asm("SLEEP");
}

void Mcu_PowerOff(void){
    OSCCONbits.IDLEN = 1U;
    asm("SLEEP");
}
