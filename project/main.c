#include "App.h"

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
void main(void) {
    Board_Init();
    Scheduler_Init();
    UART_Init();
    UART_OpenStatic();

    App_Init();
    /**
     * Wait up to 50ms for the ACK signal
     */
    if( XSMODEM_OK == xsModem_waitAck( XSMODEM_TRUE ) ){
        /**
         * When received, enter to reprogramming session
         */
        xsModem_Ack();
        App_State_AppReprogramming();
    }else{
        /**
         * Otherwise check for an app and pass control to it
         */
        App_State_AppCheck();
    }

    /**
     * Something wrong happend, set the device to a safe state
     */
    sleep();

    /**
     * Sleep didn't work, set the device into another safe state.
     */

     /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;
    while (1)
    {
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100U );
    }
}
