#ifndef _TMR1_H_
#define	_TMR1_H_

#include "Timer1_cfg.h"
#include "Timer1_types.h"
#include "Timer1_internal.h"

#ifdef	__cplusplus
extern "C" {
#endif

	#define Timer1_ClearIsr()			{ asm("BCF PIR1,0");/*Clear interrupt flag (TMR1IF)*/ }

	#define Timer1_EnableInterrupt()	{ asm("BSF PIE1,0");/*Set interrupt enable (TMR1IE)*/ }

	#define Timer1_DisableInterrupt()	{ asm("BCF PIE1,0");/*Clear interrupt enable (TMR1IE)*/ }

	extern void Timer1_Init(void);

	extern void Timer1_Open( timer1_cfg_div , timer1_cfg_clock_source );

	extern void Timer1_Load( timer1_val_t );

	#define Timer1_Start(void)			{ asm("BSF    T1CON,0");/*Enable timer (TMR1ON)*/ }

	#define Timer1_Stop(void)			{ asm("BCF    T1CON,0");/*Disable timer (TMR1ON)*/ }

	extern timer1_isr_state Timer1_GetIsrState( void );

	extern timer1_val_t Timer1_Read(void);

#ifdef	__cplusplus
}
#endif

#endif
