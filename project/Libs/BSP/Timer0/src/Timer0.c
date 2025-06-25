#include "Timer0.h"
#include "board_config.h"

void Timer0_Init(){
	T0CON = 0x00U;
	TMR0H = 0x00U;
	TMR0L = 0x00U;

    INTCONbits.TMR0IE = 0;// Interrupt Enable
    INTCONbits.TMR0IF = 0;// Interrupt flag
}

void Timer0_Open( timer0_cfg_div prescaler , timer0_cfg_clock_source clock ){
	T0CON = 0x00U;

	if( clock == TMR0_CFG_CLOCK_EXTERNAL ){
		T0CONbits.T0CS = 1U;
	}

	T0CONbits.T08BIT = 0; // 16 bit mode
	T0CONbits.T0PS = (prescaler & 0x7U); // Prescale (2 bits), from 1 to 8 units
	INTCONbits.T0IE = 0;
	INTCONbits.T0IF = 0;
    #if (TMR0_CFG_ISR_PRIORITY == TMR0_CFG_OPT_ISR_PRIORITY_HIGH)
        INTCON2bits.TMR0IP = 1U;
    #else
        INTCON2bits.TMR0IP = 0U;
    #endif
}

void Timer0_Load( timer0_val_t time ){
	time = TMR0_MAX_TICK_VAL - time;
    TMR0H = (uint8_t)( time >> 8U );
    TMR0L = (uint8_t)( time & 0xFFU );
}

timer0_isr_state Timer0_GetIsrState( void ){
	if( INTCONbits.TMR0IF ){
		return TMR0_ISR_OVERFLOW;
	}else{
		return TMR0_ISR_NONE;
	}
}

timer0_val_t Timer0_Read(){
	uint16_t tmr0_val;
	tmr0_val = (uint16_t) TMR1L;
	tmr0_val = tmr0_val | (uint16_t)(TMR1H<<8U);
	return tmr0_val;
}
