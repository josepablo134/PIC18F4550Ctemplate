#include "stdio.h"
#include "UART.h"

char msg[] = "Hello world!! [256]   \n";

void main(void) {
    Board_Init();
    UART_Init();
    UART_Open( 115200U );

    uint8_t counter = 0U;

    while(1){
        snprintf( msg, sizeof(msg), "Hello world!! [%u]\n", counter );

        while( UART_Status() ){}
        UART_TransmitAsync( 
            (const uart_byte*) msg,
            sizeof( msg ) );

            counter ++;
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100 );
    }
}