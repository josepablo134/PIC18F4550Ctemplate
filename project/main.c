/*
 * File:   main.c
 * Author: josepablocb
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

#define MSG_STR_BUF_LEN     100
#define USEC_PERIOD         500
#define MSEC_PERIOD         500
#define MSEC2USEC_PERIOD    ((uint32_t)(MSEC_PERIOD*1000U/USEC_PERIOD))
uint32_t    rt_counter;
uint16_t    counter=0;
uint8_t     msg_str_buf[MSG_STR_BUF_LEN];

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
        counter++;
        snprintf( msg_str_buf, MSG_STR_BUF_LEN, "Hello World %d\n\r", counter );
        putsUSBUSART( msg_str_buf );
    }
    LATAbits.LA4 = !PORTAbits.RA4;
}
