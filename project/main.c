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

#include "board_config.h"

#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"

void AppLoop(void);
void AppInit(void);

#define USEC_PERIOD         500
#define MSEC_PERIOD         500
#define MSEC2USEC_PERIOD    ((uint32_t)(MSEC_PERIOD*1000U/USEC_PERIOD))
uint32_t    rt_counter;

void main(void) {
    rt_counter = 0;
    Board_Init();
    AppInit();
    LATAbits.LA4 = 1U;

    USBDeviceInit();
    USBDeviceAttach();
    
    while(1)
    {
        rt_counter++;
        if( rt_counter >= MSEC2USEC_PERIOD ){
            rt_counter=0;
            AppLoop();
        }
        __delay_us( USEC_PERIOD );
        CDCTxService();
    }
}

void AppInit(void){
    /**
     * Enable PortA bit 4
     */
    ADCON1 = 0x0FU;
    CMCON = 0x07U;
    TRISAbits.RA2 = 0U;
    TRISAbits.RA3 = 0U;
    TRISAbits.RA4 = 0U;
    LATAbits.LA2 = 0U;
    LATAbits.LA3 = 0U;
    LATAbits.LA4 = 1U;
}

void AppLoop(void){
    if(USBUSARTIsTxTrfReady())
    {
        putrsUSBUSART("Hello World\n\r");
    }
    LATAbits.LA4 = !PORTAbits.RA4;
}
