#include "App.h"

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
void main(void) {
    App_state_t app_state;
    App_Init();

    if( XSMODEM_OK == xsModem_waitAck( XSMODEM_TRUE ) ){
        xsModem_Ack();
        app_state = APP_STATE_APP_REPROGRAMMING;
    }else{
        app_state = APP_STATE_APP_CHECK;
    }

    while( 1U ){
        app_state = App_mainFunction( app_state );
    }

}
