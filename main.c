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

void Timer1_Init(){
	T1CON = 0x00U;
	TMR1H = 0x00U;
	TMR1L = 0x00U;
    PIE1bits.TMR1IE = 0; // Interrupt Enable
    PIR1bits.TMR1IF = 0; // Interrupt flag
}

typedef enum {
    TMR1_CFG_DIV_1=0,
    TMR1_CFG_DIV_2,
    TMR1_CFG_DIV_4,
    TMR1_CFG_DIV_8
}timer1_cfg_div;

typedef enum {
    TMR1_CFG_CLOCK_INTERNAL, /* Use FOSC/4 clock */
    TMR1_CFG_CLOCK_EXTERNAL /* Use TMR1 external clock */
} timer1_cfg_clock_source;

void Timer1_open( timer1_cfg_div prescaler , timer1_cfg_clock_source clock ){
	T1CON = 0x00U;
    switch( clock ){
			case TMR1_CFG_CLOCK_EXTERNAL:
				{
					T1CONbits.T1RUN = 1;	// External T1 Clock
					T1CONbits.T1OSCEN = 1;	// External Osc. Enabled
					T1CONbits.TMR1CS = 1; 	// External clock
					T1CONbits.T1SYNC = 0; 	// (Negated bit) Sync. external clock with internal clock
				}
					break;
			case TMR1_CFG_CLOCK_INTERNAL:
			default:
				{
					T1CONbits.T1RUN = 0;	// External T1 Clock
					T1CONbits.T1OSCEN = 0;	// External Osc. Enabled
					T1CONbits.TMR1CS = 0; 	// External clock
				}
					break;
    }
	T1CONbits.RD16 = 0; // 16 bit mode
	T1CONbits.T1CKPS = (prescaler & 0x3U); // Prescale (2 bits), from 1 to 8 units
    
    IPR1bits.TMR1IP = 1; // High priority ISR
    PIE1bits.TMR1IE = 0; // Interrupt Enable
    PIR1bits.TMR1IF = 0; // Interrupt flag
}

void Timer1_load( uint16_t time ){
    time = (uint16_t)(-1) - time;
    TMR1H = (uint8_t)( time >> 8U );
    TMR1L = (uint8_t)( time & 0xFFU );
}

void Timer1_start(){
    /// Start incrementing timer and enable interrupt
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
}

void Timer1_stop(){
    /// Stop timer and disable interrupt
    PIE1bits.TMR1IE = 0;
    T1CONbits.TMR1ON = 0;
}

uint16_t Timer1_read(){
	uint16_t tmr1_val;
	tmr1_val = (uint16_t) TMR1L;
	tmr1_val = tmr1_val | (uint16_t)(TMR1H<<8U);
	return tmr1_val;
}

void main(void) {
    Keypad_key_t key=0;
	uint16_t timer1;

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
