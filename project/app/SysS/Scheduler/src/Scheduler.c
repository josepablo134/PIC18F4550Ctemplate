#include "Scheduler.h"

static scheduler_state_t    scheduler_state=SCHEDULER_STATE_INIT;
static systick_t            system_tick=0U;
static minitask_t           *task_list=0x00;
static uint8_t              num_of_tasks=0U;

void Scheduler_Init( void ){
    Timer0_Init();
    Timer0_Open( TIMER_CFG_DIV, TIMER_CFG_CLOCK_MODE ); // 48Mhz/4/256 = 46.875Khz
}

void Scheduler_SetTimeout( scheduler_ms_t time ){
    if( time > TIMER_MAX_MS_VALUE ){
        time = TIMER_MAX_MS_VALUE;
    }
    Timer0_Stop();
    Timer0_Load( time * TMR0_1MS_TICKS );
}

void Scheduler_StartTimer(void){
    Timer0_ClearIsr();
    Timer0_Start();
}

void Scheduler_StopTimer(void){
    Timer0_ClearIsr();
    Timer0_Stop();
}

scheduler_timeout_state Scheduler_TimeoutReached( void ){
    if( Timer0_GetIsrState() == TMR0_ISR_OVERFLOW ){
        return SCHEDULER_TIMEOUT_REACHED;
    }else{
        return SCHEDULER_TIMEOUT_NOT_REACHED;
    }
}

void Scheduler_SetTaskList( minitask_t* list, uint8_t num_tasks ){
    task_list = list;
    num_of_tasks = num_tasks;
    minitask_t *task;
    for( uint8_t task_id=0U; task_id<num_of_tasks; task_id++ ){
        task = &task_list[ task_id ];
        task->state = TASK_STATE_WAITING;
        task->count = task->offset;

        if( task->count == 0 ){
            task->state = TASK_STATE_READY;
        }

        task++;
    }
}

void Scheduler_Start(void){
    minitask_t *task;

    Scheduler_SetTimeout( SCHEDULER_SYSTICK_PERIOD );
    Scheduler_StartTimer();
    scheduler_state = SCHEDULER_STATE_RUNNING;
    while( scheduler_state == SCHEDULER_STATE_RUNNING ){            
        /**
         * Execute any READY task:
         * Instead of wasting time waiting for the timeout to be reached, use this time to execute the ready tasks.
         * As long as the SCHEDULER_SYSTICK_PERIOD is enough time to execute all tasks in the list (worst case) then
         * the scheduler will be able to react between systick events on time.
         * */
        task = task_list;
        for( uint8_t task_id=0U; task_id < num_of_tasks; task_id++ ){
            if( task->state == TASK_STATE_READY ){
                task->body();
                task->state = TASK_STATE_WAITING;
                task->count = task->period;
            }
            task++;
        }

        if( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_REACHED ){
            Scheduler_SetTimeout( SCHEDULER_SYSTICK_PERIOD );
            Scheduler_StartTimer();

            system_tick++;

            /** Check if any task is ready for execution, if so only flag the task */
            task = task_list;
            for( uint8_t task_id=0U; task_id < num_of_tasks; task_id++ ){
                task->count--;
                if( task->count == 0U ){
                    task->state = TASK_STATE_READY;
                }
                task++;
            }
        }
    }
}

void Scheduler_Stop(void){
    scheduler_state = SCHEDULER_STATE_EXIT;
}

systick_t Scheeduler_GetSysTick(void){
    return system_tick;
}
