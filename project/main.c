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
#include "inc/Board/SPI/SPI.h"
#include "inc/Board/Timer1/Timer1.h"

#include "inc/Keypad/Keypad.h"
#include "inc/utils/utils.h"

#define MSG_BUFF_LEN    30
char msg[MSG_BUFF_LEN] = "Hello world!!\n";

#define SPI_BUFF_LEN    3U
SPI_byte_t spiBuffer[MSG_BUFF_LEN];

void main(void) {
    AdcCounts_t  counts;
	SPI_Transaction_t spiTransaction = {
		.txBuffer = spiBuffer,
		.rxBuffer = spiBuffer,
		.txSize = SPI_BUFF_LEN,
		.rxSize = SPI_BUFF_LEN
	};
	memset( spiBuffer , 0xAAU , SPI_BUFF_LEN );

    Board_Init();

    Adc_Init();
    Adc_Open();

    Dio_Init();
    Dio_Open();
    Dio_WriteChannel( Dio_ch_0 , Dio_HIGH );

	SPI_Init();
	SPI_Open();

    Pwm_Init();
    Pwm_Open();

    while(1){
        Dio_WriteChannel( Dio_ch_0 , !Dio_ReadChannel(Dio_ch_0) );
        Adc_ConvertSync( Adc_Ch_0 );
        counts = Adc_getCounts();
        Pwm_SetDutyCycle( Pwm_Ch_1 , (Pwm_DutyCycle_t) counts );
        SPI_TransactionSync( &spiTransaction );
        __delay_ms( 500 );/// Debounce time
    }
}
