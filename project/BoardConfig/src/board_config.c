#include "board_config.h"
#include "Mcu.h"
#include "usb.h"

void __interrupt(high_priority) Board_HP_ISR(void){
    USBDeviceTasks();
}

void __interrupt(low_priority) Board_LP_ISR(void){
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
}