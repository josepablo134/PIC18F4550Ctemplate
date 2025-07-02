#include <stdio.h>
#include "App.h"

void putch(char c){
    UART_putch( c );
}

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
void main(void) {
    App_state_t app_state = APP_STATE_INIT;
    App_Init();
    while( 1U ){
        app_state = App_mainFunction( app_state );
    }
}
