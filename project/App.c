#include "App.h"

#define APP_RST_VECT        0x000800
#define APP_HISR            0x000808
#define APP_LISR            0x000818

void App_Init( void ){
    Board_Init();
    Scheduler_Init();
    ComM_Init();
    ComM_Open();
}

App_state_t App_mainFunction( App_state_t state ){
    switch ( state )
    {
        case APP_STATE_INIT:
            state = App_State_Init( state );
            break;
        case APP_STATE_APP_CHECK:
            state = App_State_AppCheck( state );
            break;
        case APP_STATE_APP_REPROGRAMMING:
            state = App_State_AppReprogramming( state );
            break;
        case APP_STATE_APP_SAFE_STATE:
            state = App_State_AppSafeState( state );
            break;
        default:
            break;
    }
    return state;
}

App_state_t App_State_Init( App_state_t state ){
    LATAbits.LA4 = 1U;

    if( COMM_OK == ComM_WaitAck( APP_CFG_INIT_WAIT_TIME ) ){
        ComM_SendAck();
        state = APP_STATE_APP_REPROGRAMMING;
    }else{
        state = APP_STATE_APP_CHECK;
    }

    LATAbits.LA4 = 0U;
    return state;
}

App_state_t App_State_AppCheck( App_state_t state ){
    state = APP_STATE_APP_SAFE_STATE; /// Not ready!
    return state;
}

App_state_t App_State_AppReprogramming( App_state_t state ){
    uint8_t byte;
    while( 1 ){
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100U );
    }
    return state;
}

App_state_t App_State_AppSafeState( App_state_t state ){
    sleep();
    while (1)
    {
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100U );
    }
    return state;
}
