#ifndef _TMR1_TYPES_H_
#define	_TMR1_TYPES_H_

#include "Timer1_include.h"

typedef enum {
    TMR1_CFG_DIV_1=0,
    TMR1_CFG_DIV_2,
    TMR1_CFG_DIV_4,
    TMR1_CFG_DIV_8
}timer1_cfg_div;

typedef enum {
    TMR1_CFG_CLOCK_INTERNAL, /* Use FOSC/4 clock */
    TMR1_CFG_CLOCK_EXTERNAL /* Use TMR1 external clock */
} timer1_cfg_clock_source;

typedef uint16_t timer1_val_t;

#define TMR1_CFG_OPT_ISR_PRIORITY_LOW   0U
#define TMR1_CFG_OPT_ISR_PRIORITY_HIGH  1U

#endif
