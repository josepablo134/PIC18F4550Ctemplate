#ifndef _TMR1_CFG_H_
#define	_TMR1_CFG_H_

#include "Timer1_types.h"

#define TMR1_CFG_ISR_PRIORITY       TMR1_CFG_OPT_ISR_PRIORITY_HIGH

#define TMR1_SCK_FREQ               (12000000U)
#define TMR1_SCK_DIV                (8U)
#define TMR1_FREQ                   (TMR1_SCK_FREQ/TMR1_SCK_DIV)
#define TMR1_1MS_TICKS              (TMR1_FREQ/1000U)
#define TMR1_SYSTICK_PRECHARGE      (TMR1_MAX_TICK_VAL - TMR1_1MS_TICKS)

#endif
