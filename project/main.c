#include "board_config.h"
#include "UART.h"
#include <stdio.h>

uint8_t msg[] = "Hello world, counter: [255]\n";

/// @brief Software shall not consume more than 6.25% of ROM ( 2048KB! )
void main(void) {
    Board_Init();
    UART_Init();
    UART_OpenStatic();

    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;

    
    uint8_t counter;
    uint8_t str_len;

    counter = 0U;
    while(1){
        str_len = snprintf( msg, sizeof(msg), "Hello world, counter: [%u]\n", counter++);
        UART_TransmitAsync( msg, str_len );
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 500 );
    }
}
