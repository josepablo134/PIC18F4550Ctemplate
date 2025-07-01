#include "App.h"

#define APP_RST_VECT        0x002000
#define APP_HISR            0x002008
#define APP_LISR            0x002018

static uint8_t     bldr_flash_page_buffer[APP_CFG_PAGE_BUFFER_SIZE];
static uint8_t     bldr_state;
static uint8_t     bldr_size;
static uint16_t    bldr_addr;

void App_Init( void ){
    Board_Init();
    Scheduler_Init();
    ComM_Init();
    ComM_Open();
    ComTP_Init();

    ComM_SetEventHandler( APP_CFG_COMTP_SERVICE_ID_DATA, ComTP_ComM_Service_Data );
    ComM_SetEventHandler( APP_CFG_COMTP_SERVICE_ID_SETUP, ComTP_ComM_Service_SetUp );
    ComM_SetEventHandler( APP_CFG_BOOTLDR_SERVICE_ID_CMD, App_ComM_Service_Cmd );
}

App_state_t App_mainFunction( App_state_t state ){
    switch ( state )
    {
        case APP_STATE_INIT:
            state = App_State_Init( state );
            break;
        case APP_STATE_APP_CHECK:
            state = App_State_AppCheck( state );
            break;
        case APP_STATE_APP_REPROGRAMMING:
            state = App_State_AppReprogramming( state );
            break;
        case APP_STATE_APP_SAFE_STATE:
            state = App_State_AppSafeState( state );
            break;
        default:
            break;
    }
    return state;
}

App_state_t App_State_Init( App_state_t state ){
    if( COMM_OK == ComM_WaitAck( APP_CFG_INIT_WAIT_TIME ) ){
        ComM_SendAck();
        state = APP_STATE_APP_REPROGRAMMING;
    }else{
        state = APP_STATE_APP_CHECK;
    }
    return state;
}

App_state_t App_State_AppCheck( App_state_t state ){
    STKPTR = 0U;
    asm("GOTO 0x2000");
    state = APP_STATE_APP_SAFE_STATE;
    return state;
}

App_state_t App_State_AppReprogramming( App_state_t state ){
    uint8_t byte;
    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;

    while( 1U ){
        /// Allows execution time measurement through an oscilloscope
        LATAbits.LA4 = 1U;
        ComM_mainFunction();
        LATAbits.LA4 = 0U;
    }

    LATAbits.LA4 = 0U;
    return state;
}

App_state_t App_State_AppSafeState( App_state_t state ){
    sleep();
    while (1)
    {
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100U );
    }
    return state;
}

void App_ComTP_Service_WriteBufferCallback( ComTP_Transaction_State_t comtp_state ){
    // Continue receiving commands
    bldr_state = APP_CFG_BLDR_STATE_READY;

    /// If TP transaction not valid cancel operation and return to listening mode
    if( comtp_state == COMTP_TRANSACTION_STATE_DONE_NOT_OK ){
        return;
    }

    if( FLS_OK == FlashWrite( bldr_addr, (FlashSize_t) bldr_size, (const uint8_t*) bldr_flash_page_buffer ) ){
        ComM_SendAck();
    }else{
        ComM_SendNack();
    }
}

void App_ComM_Service_Cmd( ComM_Frame_t *frame ){
    uint8_t cmd;
    if( (frame == 0x00U) || (frame->data == 0x00U) || (bldr_state == APP_CFG_BLDR_STATE_BUSY) ){
        /**
         * Invalid frame:
         *  - frame = NULL ptr
         *  - frame->datA = NULL ptr
         *  - frame.len < (CMD 1byte) + ( addr 2byte ) + ( size 1 byte ) = total of 4 bytes
         */
        ComM_SendNack();
        return;
    }
    cmd = frame->data[0U];

    switch ( cmd )
    {
        case APP_CFG_BLDR_CMD_ERASE:
            /**
             * erase( uint16_t addr, uint8_t size )
             */
            {
                if( frame->ID.len < 4U ){
                    ComM_SendNack();
                    return;
                }
                
                {
                    bldr_addr = frame->data[1U];
                    bldr_addr |= (uint16_t)frame->data[2U] << 8U;

                    bldr_size = frame->data[3U];
                }

                if( FLS_OK == FlashErase( bldr_addr, bldr_size ) ){
                    ComM_SendAck();
                }else{
                    ComM_SendNack();
                }
            }
            break;
        case APP_CFG_BLDR_CMD_WRITE:
            /**
             * write( uint16_t addr, uint8_t size )
             */
            {
                if( frame->ID.len < 4U ){
                    ComM_SendNack();
                    return;
                }
                
                {
                    bldr_addr = frame->data[1U];
                    bldr_addr |= (uint16_t)frame->data[2U] << 8U;

                    bldr_size = frame->data[3U];
                }

                if( bldr_size != APP_CFG_PAGE_BUFFER_SIZE ){
                    /// Not a valid size
                    ComM_SendNack();
                    return;
                }

                bldr_state = APP_CFG_BLDR_STATE_BUSY;
                ComM_SendAck();
                ComTP_receiveAsync( bldr_flash_page_buffer, bldr_size, App_ComTP_Service_WriteBufferCallback );
            }
            break;
        case APP_CFG_BLDR_CMD_READ:
            /**
             * read( uint16_t addr, uint8_t size )
             */
            {
                if( frame->ID.len < 4U ){
                    ComM_SendNack();
                    return;
                }
                
                {
                    bldr_addr = frame->data[1U];
                    bldr_addr |= (uint16_t)frame->data[2U] << 8U;

                    bldr_size = frame->data[3U];
                }

                if( bldr_size != APP_CFG_PAGE_BUFFER_SIZE ){
                    /// Not a valid size
                    ComM_SendNack();
                    return;
                }

                if( FLS_OK == FlashRead( bldr_addr, bldr_size, bldr_flash_page_buffer ) ){
                    ComM_SendAck();
                    ComTP_transmitSync( bldr_flash_page_buffer, bldr_size ); /// Send out the memory content
                }else{
                    ComM_SendNack();
                }
            }
            break;
        case APP_CFG_BLDR_CMD_RESET:
            /**
             * pass_control( void )
            */
            {
                ComM_SendAck();
                ComM_Lock();/// Disable ComM!
                __delay_ms( 1U );
                asm("RESET");
            }
            break;
        default:
            ComM_SendNack();
            break;
    }
}
