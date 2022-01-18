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
#include "include/flashCtl/flash.h"

#define DUMMY_BUFFER_SIZE   128U
#define DUMMY_BYTE          0x3C
#define DUMMY_ADDRESS       0x00
uint8_t dummyData[ DUMMY_BUFFER_SIZE ];

void loadBuffer( uint8_t byte ){
    for( uint16_t i=0; i<DUMMY_BUFFER_SIZE; i++ ){
        dummyData[ i ] = byte;
    }    
}

void main(void) {
    loadBuffer( DUMMY_BYTE );
    
    ASSERT( FLS_OK == FlashErase( DUMMY_ADDRESS , DUMMY_BUFFER_SIZE ) );
    ASSERT( FLS_OK == FlashWrite( DUMMY_ADDRESS , DUMMY_BUFFER_SIZE , dummyData ) );

    loadBuffer( (uint8_t) ~DUMMY_BYTE );
    
    ASSERT( FLS_OK == FlashRead( DUMMY_ADDRESS , DUMMY_BUFFER_SIZE , dummyData ) );
    ASSERT( FLS_OK == FlashValidate( DUMMY_ADDRESS , DUMMY_BUFFER_SIZE , dummyData ) );
    
    while(1){
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 200 );
    }
    return;
}
