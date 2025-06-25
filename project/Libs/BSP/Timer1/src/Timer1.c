#include "Timer1.h"
#include "board_config.h"

void Timer1_Init(){
	T1CON = 0x00U;
	TMR1H = 0x00U;
	TMR1L = 0x00U;
    PIE1bits.TMR1IE = 0; // Interrupt Enable
    PIR1bits.TMR1IF = 0; // Interrupt flag
}

void Timer1_Open( timer1_cfg_div prescaler , timer1_cfg_clock_source clock ){
	T1CON = 0x00U;

    if( clock == TMR1_CFG_CLOCK_EXTERNAL )
	{
		T1CONbits.T1RUN = 1;	// External T1 Clock
		T1CONbits.T1OSCEN = 1;	// External Osc. Enabled
		T1CONbits.TMR1CS = 1; 	// External clock
		T1CONbits.T1SYNC = 0; 	// (Negated bit) Sync. external clock with internal clock
	}

	T1CONbits.RD16 = 0; // 16 bit mode
	T1CONbits.T1CKPS = (prescaler & 0x3U); // Prescale (2 bits), from 1 to 8 units
    PIE1bits.TMR1IE = 0; // Interrupt Enable
    PIR1bits.TMR1IF = 0; // Interrupt flag
    #if (TMR1_CFG_ISR_PRIORITY == TMR1_CFG_OPT_ISR_PRIORITY_HIGH)
        IPR1bits.TMR1IP = 1;
    #else
        IPR1bits.TMR1IP = 0;
    #endif
}

void Timer1_Load( timer1_val_t time ){
	time = TMR1_MAX_TICK_VAL - time;
    TMR1H = (uint8_t)( time >> 8U );
    TMR1L = (uint8_t)( time & 0xFFU );
}

timer1_isr_state Timer1_GetIsrState( void ){
	if( PIR1bits.TMR1IF ){
		return TMR1_ISR_OVERFLOW;
	}else{
		return TMR1_ISR_NONE;
	}
}

timer1_val_t Timer1_Read(){
	uint16_t tmr1_val;
	tmr1_val = (uint16_t) TMR1L;
	tmr1_val = tmr1_val | (uint16_t)(TMR1H<<8U);
	return tmr1_val;
}
