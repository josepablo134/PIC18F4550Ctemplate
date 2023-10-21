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

#include "Pwm.h"
#include "Adc.h"

void main(void) {
AdcCounts_t  counts;
    Board_Init();
    Adc_Init();
    Adc_Open();
    Pwm_Init();
    Pwm_Open();

    LATAbits.LA4 = 1U;

    while(1){
        Adc_ConvertSync( Adc_Ch_0 );
        counts = Adc_getCounts();

        Pwm_SetDutyCycle( Pwm_Ch_1,
            (Pwm_DutyCycle_t) counts );
        
        /* Do something else */
        __delay_ms( 500 );
        LATAbits.LA4 = !PORTAbits.RA4;
    }
}
