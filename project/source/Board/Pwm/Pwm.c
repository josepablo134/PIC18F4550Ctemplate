#include "../../../inc/Board/Pwm/Pwm.h"

void Pwm_Init(void){
    /* Nothing to do here */
}

void Pwm_Open(void){
/**
 * Trying to configure the highes frequency for
 * the highest resolution possible:
 *  Datasheet (equation 15.1 and 15.3)
 * */
    CCP1CON = 0x00;
    CCPR1L = 0x00;//Clear duty cycle
    T2CON = 0x00;
    PR2 = 0xFFU;
    T2CONbits.TOUTPS = 0;//Postcale = 1
    T2CONbits.T2CKPS = 0;//Prescaler = 1
    PORTCbits.CCP1 = 0;
    TRISCbits.RC2 = 0;/// CCP1 (pwm channel 1)
    CCP1CONbits.CCP1M = 0x0C;
}
void Pwm_Enable( void ){
    T2CONbits.TMR2ON = 1;
}

void Pwm_Disable( void ){
    T2CONbits.TMR2ON = 0;
    CCPR1L = 0x00;//Clear duty cycle
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

