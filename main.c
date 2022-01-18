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
#include "include/bldrCtl/bldrCtl.h"
#include "include/flashCtl/flash.h"

extern void dummyApplication(void);

void main(void) {
    BLDR_Init();
    BLDR_MainHandler();
    dummyApplication();
    return;
}

void dummyApplication(void)
__at(0x800)
{
    while(1){
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 200 );
    }
}
