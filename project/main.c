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

#include "inc/Board/Adc/Adc.h"
#include "inc/Board/Dio/Dio.h"
#include "inc/Board/Mcu/Mcu.h"
#include "inc/Board/Pwm/Pwm.h"
#include "inc/Board/Timer1/Timer1.h"
#include "inc/Board/UART/UART.h"

#include "inc/Keypad/Keypad.h"
#include "inc/utils/utils.h"

#define MSG_BUFF_LEN    30
char msg[MSG_BUFF_LEN] = "Hello world!!\n";

void main(void) {
    Keypad_key_t key;
    AdcCounts_t  counts;
    char keyChar;

    /// Port b change interrupt configured here
    Board_Init();
    Keypad_Init();
    UART_Init();

    Adc_Init();
    Adc_Open();

    Dio_Init();
    Dio_Open();
    Dio_WriteChannel( Dio_ch_0 , Dio_HIGH );

    Pwm_Init();
    Pwm_Open();
    Pwm_Enable();

    UART_Open( 115200 );
    UART_TransmitSync( (const uart_byte*) msg , strlen( msg ) );
    strcpy( msg , "Key pressed : [x] [xxxx]\n" );
    
    Keypad_Open();

    /// Enable change on port b interrupt
    INTCON2 = 0x00U;
    INTCON2bits.RBIP = 1U;/// High priority interrupt
    PORTB = 0x00;///In order to clear RBIF (datasheet page 116)
    INTCONbits.RBIF = 0U;
    INTCONbits.RBIE = 1U;
    while(1){
        counts = Adc_getCounts();
        Pwm_SetDutyCycle( Pwm_Ch_1 , (Pwm_DutyCycle_t) counts );
        Adc_ConvertAsync( Adc_Ch_0 );
        Dio_WriteChannel( Dio_ch_0 , !Dio_ReadChannel(Dio_ch_0) );

        key = Keypad_captureKey();
        keyChar = Keypad_Key2Ascii( key );
        if( KEYPAD_CFG_INVALID_CHAR != keyChar ){
            msg[ 15 ] = keyChar;
            msg[ 19 ] = int2hex_ascii[ (counts >> 12U) & 0x0F ];
            msg[ 20 ] = int2hex_ascii[ (counts >> 8U) & 0x0F ];
            msg[ 21 ] = int2hex_ascii[ (counts >> 4U) & 0x0F ];
            msg[ 22 ] = int2hex_ascii[ (counts & 0x0F) ];
            UART_TransmitSync( (const uart_byte*) msg , strlen(msg) );
        }

        Mcu_Sleep(); /// Wait for port b interrupt
        __delay_ms( 30 );/// Debounce time
    }
}
