/*
 * File:   main.c
 * Author: josepablocb
 *
 * Created on December 14, 2021, 7:24 PM
 */
#include <stdbool.h>
#include <stdint.h>
#include "include/config/board_config.h"
#include "include/config/project_macros.h"

void main(void) {
    while(1){
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms(50);
    }
    return;
}
