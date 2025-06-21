#include "UART.h"

char msg[] = "Hello world!!\n";

void main(void) {
    Board_Init();
    UART_Init();
    UART_Open( 115200U );

    while(1){
        LATAbits.LA4 = !PORTAbits.RA4;
        while( UART_Status() ){}
        UART_TransmitAsync( 
            (const uart_byte*) msg,
            sizeof( msg ) );
        /* Do something else */
        __delay_ms( 100 );
    }
}