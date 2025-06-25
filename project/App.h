#ifndef APP_H
#define APP_H

    typedef enum App_state_t{
        APP_STATE_INIT,
        APP_STATE_APP_CHECK,
        APP_STATE_APP_REPROGRAMMING,
        APP_STATE_APP_SAFE_STATE,
    }App_state_t;

    extern void App_Init( void );
    extern App_state_t App_mainFunction( App_state_t );
    extern App_state_t App_State_Init( App_state_t );
    extern App_state_t App_State_AppCheck( App_state_t );
    extern App_state_t App_State_AppReprogramming( App_state_t );
    extern App_state_t App_State_AppSafeState( App_state_t );

#endif
