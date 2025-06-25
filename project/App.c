#include "App.h"
#include "ComM.h"
#include "Timer0.h"

#define APP_RST_VECT        0x000800
#define APP_HISR            0x000808
#define APP_LISR            0x000818

uint8_t com_buffer[ COMM_CFG_PAYLOAD_DATA_LEN ];
ComM_Payload_t frame = {
    .data = com_buffer
};

void App_Init( void ){
    Board_Init();
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
    uint32_t counter = _XTAL_FREQ / (1000U / 10U); /// Wait 10ms in form of clock ticks

    com_buffer[0U] = 0x00U;

    LATAbits.LA4 = 1U;

    if( COMM_OK == ComM_WaitAck( counter ) ){
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
    UART_SetPollingReceive( com_buffer, 1U );
    while( 1 ){
        if( ! ( UART_PollReceive( 1U ) & RX_BUSY ) ){ /// Reception completed
            LATAbits.LA4 = !PORTAbits.RA4;
            UART_TransmitSync( com_buffer, 1U );
            UART_SetPollingReceive( com_buffer, 1U );
        }
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
