#ifndef _TMR0_H_
#define	_TMR0_H_

#include "Timer0_cfg.h"
#include "Timer0_types.h"
#include "Timer0_internal.h"

#ifdef	__cplusplus
extern "C" {
#endif

	#define Timer0_ClearIsr()			{ asm("BCF INTCON,2");/*Clear interrupt flag (TMR0IF)*/ }

	#define Timer0_EnableInterrupt()	{ asm("BSF INTCON,5");/*Set interrupt enable (TMR0IE)*/ }

	#define Timer0_DisableInterrupt()	{ asm("BCF INTCON,5");/*Clear interrupt enable (TMR0IE)*/ }

	extern void Timer0_Init(void);

	extern void Timer0_Open( timer0_cfg_div , timer0_cfg_clock_source );

	extern void Timer0_Load( timer0_val_t );

	#define Timer0_Start(void)			{ asm("BSF    T0CON,7");/*Enable timer (TMR0ON)*/ }

	#define Timer0_Stop(void)			{ asm("BCF    T0CON,7");/*Disable timer (TMR0ON)*/ }

	extern timer0_isr_state Timer0_GetIsrState( void );

	extern timer0_val_t Timer0_Read(void);

#ifdef	__cplusplus
}
#endif

#endif
