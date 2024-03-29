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
#include "usb_device_hid.h"

/*******************************************************************************
 * HID Report Descriptor - this describes the data format of the reports that
 * are sent between the host and the device.
 *
 * In this example there are only one INPUT report.  This report descriptor can
 * be generated using the HID descriptor tool available at www.usb.org.
 ******************************************************************************/
const struct{uint8_t report[HID_RPT01_SIZE];}hid_rpt01=
{
    {
        0x05, 0x01, /* Usage Page (Generic Desktop)             */
        0x09, 0x02, /* Usage (Mouse)                            */
        0xA1, 0x01, /* Collection (Application)                 */
        0x09, 0x01, /*  Usage (Pointer)                         */
        0xA1, 0x00, /*  Collection (Physical)                   */
        0x05, 0x09, /*      Usage Page (Buttons)                */
        0x19, 0x01, /*      Usage Minimum (01)                  */
        0x29, 0x03, /*      Usage Maximum (03)                  */
        0x15, 0x00, /*      Logical Minimum (0)                 */
        0x25, 0x01, /*      Logical Maximum (1)                 */
        0x95, 0x03, /*      Report Count (3)                    */
        0x75, 0x01, /*      Report Size (1)                     */
        0x81, 0x02, /*      Input (Data, Variable, Absolute)    */
        0x95, 0x01, /*      Report Count (1)                    */
        0x75, 0x05, /*      Report Size (5)                     */
        0x81, 0x01, /*      Input (Constant)    ;5 bit padding  */
        0x05, 0x01, /*      Usage Page (Generic Desktop)        */
        0x09, 0x30, /*      Usage (X)                           */
        0x09, 0x31, /*      Usage (Y)                           */
        0x15, 0x81, /*      Logical Minimum (-127)              */
        0x25, 0x7F, /*      Logical Maximum (127)               */
        0x75, 0x08, /*      Report Size (8)                     */
        0x95, 0x02, /*      Report Count (2)                    */
        0x81, 0x06, /*      Input (Data, Variable, Relative)    */
        0xC0, 0xC0  /* End Collection,End Collection            */
    }
};

/*******************************************************************************
 * Report Data Types - These typedefs will mirror the reports defined in the
 * HID report descriptor so that the application has an easy way to see what
 * the report will look like as well as access/modify each of the members of the
 * report.
 ******************************************************************************/

/* INPUT report - this structure will represent the only INPUT report in the HID
 * descriptor.
 */
typedef struct PACKED
{
    /* The first INPUT item is the following:
     *   0x05, 0x09,    //Usage Page (Buttons)
     *   0x19, 0x01,    //Usage Minimum (01)
     *   0x29, 0x03,    //Usage Maximum (03)
     *   0x15, 0x00,    //Logical Minimum (0)
     *   0x25, 0x01,    //Logical Maximum (1)
     *   0x95, 0x03,    //Report Count (3)
     *   0x75, 0x01,    //Report Size (1)
     *   0x81, 0x02,    //Input (Data, Variable, Absolute)
     *
     * The usage page is buttons
     * The report size is 1 (1-bit)
     * The report count is 3, thus 3 1-bit items
     * The Usage Min is 1 and the Usage maximum is 3, thus buttons 1-3, also
     *   call the primary, secondary, and tertiary buttons.
     *
     * The second INPUT item comes from the fact that the report must be byte
     * aligned, so we need to pad the previous 3-bit report with 5-bits of
     * constant(filler) data.
     *   0x95, 0x01,    //Report Count (1)
     *   0x75, 0x05,    //Report Size (5)
     *   0x81, 0x01,    //Input (Constant)
     */
    union PACKED
    {
        struct PACKED
        {
            unsigned button1   :1;
            unsigned button2   :1;
            unsigned button3   :1;
            unsigned :5;
        };
        struct PACKED
        {
            unsigned primary   :1;
            unsigned secondary :1;
            unsigned tertiary  :1;
            unsigned :5;
        };
        uint8_t value;
    } buttons;

    /* The final INPUT item is the following:
     *   0x05, 0x01,    //Usage Page (Generic Desktop)
     *   0x09, 0x30,    //Usage (X)
     *   0x09, 0x31,    //Usage (Y)
     *   0x15, 0x81,    //Logical Minimum (-127)
     *   0x25, 0x7F,    //Logical Maximum (127)
     *   0x75, 0x08,    //Report Size (8)
     *   0x95, 0x02,    //Report Count (2)
     *   0x81, 0x06,    //Input (Data, Variable, Relative)
     *
     * The report size is 8 (8-bit)
     * The report count is 2, thus 2 bytes of data.
     * The first usage is (X) and the second is (Y) so the first byte will
     *   represent the X mouse value, and the second the Y value.
     * The logical min/max determines the bounds for X and Y, -127 to 127.
     * The INPUT type is relative so each report item is relative to the last
     *   report item.  So reporting "-1" for X means that since the last report
     *   was sent, the mouse has move left
     */
    uint8_t x;
    uint8_t y;
} MOUSE_REPORT;

#define MOUSE_REPORT_DATA_BUFFER_ADDRESS      __at(0x500)

static MOUSE_REPORT mouseReport MOUSE_REPORT_DATA_BUFFER_ADDRESS;
USB_HANDLE usbHandle;

void AppLoop(void);
void AppInit(void);

#define USEC_PERIOD         (500UL)
#define MSEC_PERIOD         (10UL)
#define MSEC2USEC_PERIOD    ((uint32_t)(MSEC_PERIOD*1000UL/USEC_PERIOD))
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

    usbHandle = NULL;
    mouseReport.buttons.value = 0U;
    mouseReport.x = mouseReport.y = 0U;
}

typedef enum mouse_state_t{
    MOUSE_UP=0,
    MOUSE_RIGHT,
    MOUSE_DOWN,
    MOUSE_LEFT,
}mouse_state_t;

mouse_state_t mouseState=MOUSE_UP;
#define MOUSE_MOVE_COUNT_MAX    (400U)
uint16_t mouse_move_count=MOUSE_MOVE_COUNT_MAX;

void AppLoop(void){
    /**
     * We can only send a report if the last report has been sent.
     */
    if(HIDTxHandleBusy( usbHandle ) == false)
    {
        mouse_move_count++;
        if( mouse_move_count >= MOUSE_MOVE_COUNT_MAX ){
            mouse_move_count = 0;
        }
        switch ( mouseState )
        {
            case MOUSE_UP:{
                mouseReport.x = 0;
                mouseReport.y = (uint8_t) -1;
                if( !mouse_move_count ){mouseState = MOUSE_RIGHT;}
            }
                break;
            case MOUSE_RIGHT:{
                mouseReport.x = 1;
                mouseReport.y = 0;
                if( !mouse_move_count ){mouseState = MOUSE_DOWN;}
            }
                break;
            case MOUSE_DOWN:{
                mouseReport.x = 0;
                mouseReport.y = 1;
                if( !mouse_move_count ){mouseState = MOUSE_LEFT;}
            }
                break;
            case MOUSE_LEFT:{
                mouseReport.x = (uint8_t) -1;
                mouseReport.y = 0;
                if( !mouse_move_count ){mouseState = MOUSE_UP;}
            }
                break;
            default:{
                mouseState = MOUSE_UP;
                mouse_move_count = 0;
            }
            break;
        }
        usbHandle = HIDTxPacket(
                        HID_EP,
                        (uint8_t*)&mouseReport,
                        sizeof(mouseReport)
                    );
    }
    LATAbits.LA4 = !PORTAbits.RA4;
}


void USB_DEVICE_HID_IDLE_RATE_CALLBACK(uint8_t reportId, uint8_t idleRate){
    
}
