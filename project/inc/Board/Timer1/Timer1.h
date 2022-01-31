#ifndef _TMR1_H_
#define	_TMR1_H_

#include "Timer1_cfg.h"
#include "Timer1_types.h"

#ifdef	__cplusplus
extern "C" {
#endif

void Timer1_Init(void);

void Timer1_open( timer1_cfg_div , timer1_cfg_clock_source );

void Timer1_load( timer1_val_t );

void Timer1_start(void);

void Timer1_stop(void);

timer1_val_t Timer1_read(void);

#ifdef	__cplusplus
}
#endif

#endif
