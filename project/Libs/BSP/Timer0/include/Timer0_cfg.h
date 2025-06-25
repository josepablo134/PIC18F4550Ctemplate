#ifndef _TMR0_CFG_H_
#define	_TMR0_CFG_H_

#include "Timer0_types.h"

#define TMR0_CFG_ISR_PRIORITY       TMR0_CFG_OPT_ISR_PRIORITY_HIGH

#define TMR0_SCK_FREQ               (12000000U)
#define TMR0_SCK_DIV                (256U)
#define TMR0_FREQ                   (TMR0_SCK_FREQ/TMR0_SCK_DIV)
#define TMR0_1MS_TICKS              (TMR0_FREQ/1000U)
#define TMR0_SYSTICK_PRECHARGE      (TMR0_MAX_TICK_VAL - TMR0_1MS_TICKS)

#endif
