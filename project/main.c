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
    AdcCounts_t  counts;

    Board_Init();
    UART_Init();
    UART_Open( 115200 );

    Adc_Init();
    Adc_Open();

    Dio_Init();
    Dio_Open();
    Dio_WriteChannel( Dio_ch_0 , Dio_HIGH );

    Pwm_Init();
    Pwm_Open();
    Pwm_Enable();

    while(1){
        Dio_WriteChannel( Dio_ch_0 , !Dio_ReadChannel(Dio_ch_0) );
        Adc_ConvertSync( Adc_Ch_0 );
        counts = Adc_getCounts();
        Pwm_SetDutyCycle( Pwm_Ch_1 , (Pwm_DutyCycle_t) counts );
        UART_TransmitSync( (const uart_byte*) msg , strlen(msg) );
        Mcu_Sleep(); /// Wait for any interrupt
        __delay_ms( 30 );/// Debounce time
    }
}
