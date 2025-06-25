#include "App.h"
#include "Scheduler.h"

void Task0(void){
    asm("BTG    LATA,4");
}

void Task1(void){
    asm("BTG    LATA,0");
}

#define NUM_OF_TASKS    (2U)
minitask_t task_list[NUM_OF_TASKS] = {
    {
        .body = Task0,
        .period = 2U,
        .offset = 0U,
    },
    {
        .body = Task1,
        .period = 2U,
        .offset = 1U
    }
};

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
/// @param  
void main(void) {
    App_state_t app_state;
    app_state = APP_STATE_INIT;

    App_Init();
    Scheduler_Init();
    Scheduler_SetTaskList( task_list, NUM_OF_TASKS );
    Scheduler_Start();

    while( 1 ){
        app_state = App_mainFunction( app_state );

        Scheduler_SetTimeout( 5U );
        Scheduler_StartTimer();
        while( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ){}
        asm("BTG    LATA,4");// Toggle LAT4
    }
}
