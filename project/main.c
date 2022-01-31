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

#include "inc/Board/board_config.h"
#include "inc/Board/UART/UART.h"
#include "inc/Board/Timer1/Timer1.h"
#include "inc/Board/flashCtl/flash.h"
#include "inc/project_macros.h"

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
    Keypad_key_t    key=0;
	uint16_t        timer1;

    Board_Init();
    
	Timer1_Init();
	Timer1_open( TMR1_CFG_DIV_1 , TMR1_CFG_CLOCK_EXTERNAL );
    Timer1_load( 65503U );
	Timer1_start();

    UART_init();
    UART_open(115200);
    
    Keypad_init();
#if 0    
    strcpy( msg , "Hello world!\r\n" );
    UART_TransmitSync( msg , strlen( msg ) );
    strcpy( msg , "Key pressed : [x] [xx] [0xxxxx]\r\n" );
#endif
    while(1){
#if 0
        key = Keypad_captureKey();
        //msg[ 15 ] = Keypad_key2ascii( key );
        msg[ 19 ] = int2hex_ascii[ key >> 4U ];
        msg[ 20 ] = int2hex_ascii[ key & 0x0F ];

		timer1 = Timer1_read();

		msg[ 26 ] = int2hex_ascii[ (0xFU) & (timer1 >> 12U) ];
		msg[ 27 ] = int2hex_ascii[ (0xFU) & (timer1 >> 8U) ];
		msg[ 28 ] = int2hex_ascii[ (0xFU) & (timer1 >> 4U) ];
		msg[ 29 ] = int2hex_ascii[ (0xFU) & timer1 ];
        
        UART_TransmitSync( msg , strlen( msg ) );
#endif
        LATAbits.LA2 = !PORTAbits.RA2;
        __delay_ms( 100 );
    }
}
