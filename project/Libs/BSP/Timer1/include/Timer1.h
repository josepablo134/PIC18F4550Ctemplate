#ifndef _TMR1_H_
#define	_TMR1_H_

#include "Timer1_cfg.h"
#include "Timer1_types.h"

#ifdef	__cplusplus
extern "C" {
#endif

	extern void Timer1_Init(void);

	extern void Timer1_Open( timer1_cfg_div , timer1_cfg_clock_source );

	extern void Timer1_Load( timer1_val_t );

	extern void Timer1_Start(void);

	extern void Timer1_Stop(void);

	extern timer1_val_t Timer1_Read(void);

#ifdef	__cplusplus
}
#endif

#endif
