#ifndef _TMR0_TYPES_H_
#define	_TMR0_TYPES_H_

    #include "Timer0_include.h"

    typedef enum {
        TMR0_CFG_DIV_2=0,
        TMR0_CFG_DIV_4,
        TMR0_CFG_DIV_8,
        TMR0_CFG_DIV_16,
        TMR0_CFG_DIV_32,
        TMR0_CFG_DIV_64,
        TMR0_CFG_DIV_128,
        TMR0_CFG_DIV_256
    }timer0_cfg_div;

    typedef enum {
        TMR0_CFG_CLOCK_INTERNAL, /* Use FOSC/4 clock */
        TMR0_CFG_CLOCK_EXTERNAL /* Use TMR1 external clock */
    } timer0_cfg_clock_source;

    typedef enum {
        TMR0_ISR_NONE,
        TMR0_ISR_OVERFLOW
    }timer0_isr_state;

    typedef uint16_t timer0_val_t;

    #define TMR0_CFG_OPT_ISR_PRIORITY_LOW       (0U)
    #define TMR0_CFG_OPT_ISR_PRIORITY_HIGH      (1U)

    #define TMR0_MAX_TICK_VAL                    ((timer0_val_t) 0xFFFFU)

#endif
