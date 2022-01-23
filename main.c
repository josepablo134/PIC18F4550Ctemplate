/*
 * File:   main.c
 * Author: josepablocb
 *
 * Created on December 14, 2021, 7:24 PM
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "include/config/board_config.h"
#include "include/config/project_macros.h"
#include "include/bldrCtl/bldrCtl.h"
#include "include/flashCtl/flash.h"
#include "include/UART/UART.h"

typedef uint8_t Keypad_key_t;

void Keypad_init(){
    /// Internal weak pull-up enabled
    // Only affects input bits
    PORTB = 0x00;
    LATB = 0x00;
    TRISB = 0x0F;
    INTCON2bits.NOT_RBPU = 0;
}

Keypad_key_t Keypad_captureKey(){
    Keypad_key_t key;

    TRISB = 0x0F;
    LATB = 0x0F;///Pull-up control
    key = PORTB & 0x0F;

    TRISB = 0xF0;
    LATB = 0xF0;///Pull-up control
    key |= PORTB & 0xF0;
    
    /// Pressed = 1, unpress = 0
    key = ~key;
    return key;
}

char Keypad_key2ascii( Keypad_key_t key ){
    const char key2ascii[ 16U ] = {
        '1','2','3','A',
        '4','5','6','B',
        '7','8','9','C',
        '*','0','#','D'
    };
    char kchar = ' ';
    if( key ){
        uint8_t index;
        uint8_t byte = key & 0x0F;

        index = ( ( byte > 8 )? 3 : byte>>1U ) + 1U;
        byte = key >> 4U;
        index *= ( ( byte > 8 )? 3 : byte>>1U ) + 1U;
        
        index -= 1;
        kchar = key2ascii[ index ];
    }
    return kchar;
}

#define MSG_BUFF_LEN    100
char msg[MSG_BUFF_LEN];

const char int2hex_ascii[] = {
    '0','1','2','3',
    '4','5','6','7',
    '8','9','A','B',
    'C','D','E','F'
};

void main(void) {
    Keypad_key_t key=0;
    Board_Init();
    UART_init();
    UART_open(115200);
    Keypad_init();
    
    strcpy( msg , "Hello world!\r\n" );
    UART_Transmit( msg , strlen( msg ) );
        while( UART_Status() ){}
    
    strcpy( msg , "Key pressed : [x] [xx]\r\n" );
    while(1){
        key = Keypad_captureKey();
        while( UART_Status() ){}
        //msg[ 15 ] = Keypad_key2ascii( key );
        msg[ 19 ] = int2hex_ascii[ key >> 4U ];
        msg[ 20 ] = int2hex_ascii[ key & 0x0F ];
        
        UART_Transmit( msg , strlen( msg ) );
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 500 );
    }
}
