#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "Scheduler_include.h"
#include "Scheduler_types.h"
#include "Scheduler_cfg.h"

#ifdef	__cplusplus
extern "C" {
#endif

    /** Initialize drivers and buffers */
    extern void Scheduler_Init( void );

    /** Set timeout in ms */
    extern void Scheduler_SetTimeout( scheduler_ms_t );

    /** Start timeout count */
    extern void Scheduler_StartTimer(void);

    /** Stop timeout count */
    extern void Scheduler_StopTimer(void);

    /** Check if timeout is already reached */
    extern scheduler_timeout_state Scheduler_TimeoutReached( void );

    /** Provide the list of tasks */
    extern void Scheduler_SetTaskList( minitask_t* , uint8_t );

    extern systick_t Scheduler_GetSysTick(void);

    /** Start scheduler */
    extern void Scheduler_Start(void);

    /** Stop scheduler */
    extern void Scheduler_Stop(void);

#ifdef	__cplusplus
}
#endif
    
#endif
