#include "App.h"

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
void main(void) {
    App_state_t app_state;
    app_state = APP_STATE_INIT;

    App_Init();

    while( 1 ){
        app_state = App_mainFunction( app_state );
    }
}
