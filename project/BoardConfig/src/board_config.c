#include "board_config.h"
#include "Mcu.h"
#include "UART_internal.h"
#include "Timer1_internal.h"
#include "Adc_internal.h"
#include "SPI_internal.h"
#include "IIC_internal.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    if( PIR1bits.TMR1IF ){
        TMR1_ISR();
    }
    if( INTCONbits.RBIF ){
        INTCONbits.RBIF = 0U;
    }
}

void __interrupt(low_priority) Board_LP_ISR(void){
    UART_ISR();
    ADC_ISR();
    SPI_ISR();
    IIC_ISR();
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

    /**
     * Enable Timer2 clock (FOSC/4)
     * Used by PWM and SPI/IIC modules
     */
    T2CON = 0x00;
    T2CONbits.TOUTPS = 0;// Postcale = 1
    T2CONbits.T2CKPS = 0;// Prescaler = 1
    T2CONbits.TMR2ON = 1;// Enable Timer2 clock
}