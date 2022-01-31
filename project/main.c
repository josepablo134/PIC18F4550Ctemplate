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

#include "inc/Board/UART/UART.h"
#include "inc/Board/Timer1/Timer1.h"
#include "inc/Keypad/Keypad.h"
#include "inc/utils/utils.h"

#define MSG_BUFF_LEN    100
char msg[MSG_BUFF_LEN];

void main(void) {
    Board_Init();
    
	Timer1_Init();
	Timer1_open( TMR1_CFG_DIV_1 , TMR1_CFG_CLOCK_EXTERNAL );
    Timer1_load( 65503U );
	Timer1_start();

    while(1){
        LATAbits.LA2 = !PORTAbits.RA2;
        __delay_ms( 100 );
    }
}
