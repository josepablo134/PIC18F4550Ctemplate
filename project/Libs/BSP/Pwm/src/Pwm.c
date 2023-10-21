#include "Pwm.h"

void Pwm_Init(void){
    /* Nothing to do here */
}

void Pwm_Open(void){
/**
 * Trying to configure the highes frequency for
 * the highest resolution possible:
 *  Datasheet (equation 15.1 and 15.3)
 * 
 * Assuming Timer2 clock is already configured by Board driver
 * */
    CCP1CON = 0x00;
    CCPR1L = 0x00;//Clear duty cycle
    PR2 = 0xFFU;
    PORTCbits.CCP1 = 0;
    TRISCbits.RC2 = 0;/// CCP1 (pwm channel 1)
    CCP1CONbits.CCP1M = 0x0C;
}

void Pwm_SetDutyCycle( Pwm_Channel_t channel , Pwm_DutyCycle_t dutycycle ){
    switch( channel ){
        case Pwm_Ch_1:
            CCP1CONbits.DC1B = dutycycle & 0x03;
            CCPR1L = ( dutycycle >> 2U ) & 0xFFU;
            break;
        case Pwm_Ch_2:
            CCP2CONbits.DC2B = dutycycle & 0x03;
            CCPR2L = ( dutycycle >> 2U ) & 0xFFU;
            break;
        default:
            break;
    }
}

