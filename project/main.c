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

#include "inc/Board/Mcu/Mcu.h"
#include "inc/Board/UART/UART.h"
#include "inc/Board/Timer1/Timer1.h"
#include "inc/Keypad/Keypad.h"
#include "inc/utils/utils.h"

#include "inc/Board/Adc/Adc.h"
#include "inc/Board/Pwm/Pwm.h"

#define MSG_BUFF_LEN    100
char msg[MSG_BUFF_LEN] = "Hello world!!\n";

void main(void) {
    Keypad_key_t key;
    char keyChar;
    
    /// Port b change interrupt configured here
    Board_Init();
    Keypad_Init();
    UART_Init();
    
    Pwm_Init();
    Pwm_Open();
    Pwm_Enable();
    Pwm_SetDutyCycle( Pwm_Ch_1 , (Pwm_DutyCycle_t) 512U );

    UART_Open( 115200 );
    UART_TransmitSync( (const uart_byte*) msg , strlen( msg ) );
    strcpy( msg , "Key pressed : [x] [xx]\r\n" );
    
    Keypad_Open();

    /// Enable change on port b interrupt
    INTCON2 = 0x00U;
    INTCON2bits.RBIP = 1U;/// High priority interrupt
    PORTB = 0x00;///In order to clear RBIF (datasheet page 116)
    INTCONbits.RBIF = 0U;
    INTCONbits.RBIE = 1U;
    while(1){
        LATAbits.LA3 = !PORTAbits.RA3;

        key = Keypad_captureKey();
        keyChar = Keypad_Key2Ascii( key );
        if( KEYPAD_CFG_INVALID_CHAR != keyChar ){
            msg[ 15 ] = keyChar;
            msg[ 19 ] = int2hex_ascii[ key >> 4U ];
            msg[ 20 ] = int2hex_ascii[ key & 0x0F ];
            UART_TransmitSync( (const uart_byte*) msg , strlen( msg ) );
        }
        
        Mcu_Sleep(); /// Wait for port b interrupt
        __delay_ms( 30 );/// Debounce time
    }
}
