#ifndef _SCHEDULER_TYPES_H_
#define	_SCHEDULER_TYPES_H_

    #include "Scheduler_include.h"

    typedef uint16_t scheduler_ms_t;

    typedef uint32_t systick_t;

    typedef enum scheduler_state_t{
        SCHEDULER_STATE_INIT=0U,
        SCHEDULER_STATE_RUNNING,
        SCHEDULER_STATE_EXIT,
    }scheduler_state_t;

    typedef enum scheduler_timeout_state{
        SCHEDULER_TIMEOUT_NOT_REACHED=0U,
        SCHEDULER_TIMEOUT_REACHED,
    }scheduler_timeout_state;

    typedef enum minitask_state_t{
        TASK_STATE_WAITING=0U,
        TASK_STATE_READY
    }minitask_state_t;

    typedef void (*task_body)(void);

    typedef struct minitask_t{
        minitask_state_t    state;
        const uint16_t      period;
        const uint16_t      offset;
        task_body           body;
        uint32_t            count;
    }minitask_t;

#endif
