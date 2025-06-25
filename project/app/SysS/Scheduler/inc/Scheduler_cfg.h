#ifndef _SCHEDULER_CFG_H_
#define	_SCHEDULER_CFG_H_

    #include "Scheduler_include.h"

    #define TIMER_CFG_DIV               TMR0_CFG_DIV_256
    #define TIMER_CFG_CLOCK_MODE        TMR0_CFG_CLOCK_INTERNAL

    #define TIMER_MAX_MS_VALUE          ( (uint16_t)( 65536U / (uint32_t)TMR0_1MS_TICKS ) )

    /** Tick Period in MS */
    #define SCHEDULER_SYSTICK_PERIOD    (1U)

#endif
