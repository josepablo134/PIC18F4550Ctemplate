#include "App.h"

#define APP_RST_VECT        0x002000
#define APP_HISR            0x002008
#define APP_LISR            0x002018

static xsmodem_packet_t bldr_packet;
static uint8_t bldr_flash_page_buffer[APP_CFG_PAGE_BUFFER_SIZE];


/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/

/**
 * Get 4 bytes from UART (addr and ~addr) and return them as address
 */
static uint8_t App_Bldr_GetAddr( uint16_t *addr );

/**
 * Get 2 bytes from UART (len and ~len) and return them as length
 */
static uint8_t App_Bldr_GetLength( uint8_t *length );

/**
 * Send the ASCII char for CANCEL through UART
 */
inline static void App_sendCancel(uint8_t err);

/**
 * Read a block (up to 128 bytes) of program Flash
 */
inline static void App_Cmd_ReadBlock(void);

/**
 * Erase a block (up to 128 bytes) of program Flash
 */
inline static void App_Cmd_EraseBlock(void);

/**
 * Program a block (128 bytes) of program Flash
 */
inline static void App_Cmd_ProgramBlock(void);


/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/

void App_Init( void ){
    bldr_packet.length = APP_CFG_PAGE_BUFFER_SIZE;
    bldr_packet.payload = bldr_flash_page_buffer;
}

void App_State_AppCheck( void ){
    STKPTR = 0U;
    asm("GOTO 0x2000");
}

void App_State_AppReprogramming( void ){
    uint8_t     cmd;
    while( 1U ){
        cmd = UART_getch();
        switch (cmd)
        {
            case 'R':
                /**
                 * READ MEMORY
                */
                App_Cmd_ReadBlock();
                break;
            case 'E':
                /**
                 * ERASE MEMORY
                */
               App_Cmd_EraseBlock();
                break;
            case 'P':
                /**
                 * PROGRAM MEMORY
                 */
                App_Cmd_ProgramBlock();
                break;
            case 0x0A:
                /**
                 * SYSTEM RESET
                 */
                xsModem_Ack();
                __delay_ms( 10U );
                reset();
                break;
            case 'A':
                /** 
                 * PING
                */
                UART_putch( 'A' );
                break;
            default:
                break;
        }
    }
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/

static uint8_t App_Bldr_GetAddr( uint16_t *addr ){
    uint16_t addr_n;
    *addr = UART_getch();
    *addr |= ((uint16_t)UART_getch()<<8U);

    addr_n = UART_getch();
    addr_n |= ((uint16_t)UART_getch()<<8U);

    if( *addr != (uint16_t) ~addr_n ){
        return 1;
    }
    return 0;
}

static uint8_t App_Bldr_GetLength( uint8_t *length ){
    uint8_t length_n;
    *length = UART_getch();
    length_n = UART_getch();

    if( *length != (uint8_t) ~length_n ){
        return 1;
    }
    return 0;
}

inline static void App_sendCancel( uint8_t err ){
    UART_putch( err );
}

inline static void App_Cmd_ReadBlock(void){
    uint16_t    bldr_addr;
    uint8_t     bldr_len;

    if( App_Bldr_GetAddr( &bldr_addr ) ){
        return App_sendCancel( APP_ERR_CHAR );
    }

    if( App_Bldr_GetLength( &bldr_len ) || ( bldr_len > APP_CFG_PAGE_BUFFER_SIZE ) ){
        return App_sendCancel( APP_ERR_CHAR + 1 );
    }

    if( FLS_NOT_OK == FlashRead( bldr_addr, bldr_len, bldr_flash_page_buffer ) ){
        return App_sendCancel( APP_ERR_CHAR + 2 );
    }

    /// Command accepted
    xsModem_Ack();

    /// Packet transmission
    bldr_packet.length = bldr_len;
    xsModem_Transmit( &bldr_packet, XSMODEM_TRUE );
}

inline static void App_Cmd_EraseBlock(void){
    uint16_t    bldr_addr;
    uint8_t     bldr_len;

    if( App_Bldr_GetAddr( &bldr_addr ) ){
        return App_sendCancel( APP_ERR_CHAR );
    }

    if( App_Bldr_GetLength( &bldr_len ) || ( bldr_len > APP_CFG_PAGE_BUFFER_SIZE ) ){
        return App_sendCancel( APP_ERR_CHAR + 1 );
    }

    if( FLS_NOT_OK == FlashErase( bldr_addr, bldr_len ) ){
        return App_sendCancel( APP_ERR_CHAR + 2 );
    }

    xsModem_Ack();
}

inline static void App_Cmd_ProgramBlock(void){
    uint16_t    bldr_addr;
    uint8_t     bldr_len;

    if( App_Bldr_GetAddr( &bldr_addr ) ){
        return App_sendCancel( APP_ERR_CHAR );
    }

    /// Command accepted
    xsModem_Ack();

    /// Length is determined by the lenght of the packet's payload
    if( xsModem_Receive( &bldr_packet, XSMODEM_TRUE ) == XSMODEM_NOT_OK ){
        return App_sendCancel( APP_ERR_CHAR + 1 );
    }

    bldr_len = bldr_packet.length;

    if( FLS_NOT_OK == FlashErase( bldr_addr, bldr_len ) ){
        return App_sendCancel( APP_ERR_CHAR + 2 );
    }

    if( FLS_NOT_OK == FlashWrite( bldr_addr, bldr_len, bldr_flash_page_buffer ) ){
        return App_sendCancel( APP_ERR_CHAR + 3 );
    }

    if( FLS_NOT_OK == FlashValidate( bldr_addr, bldr_len, bldr_flash_page_buffer ) ){
        return App_sendCancel( APP_ERR_CHAR + 4 );
    }

    xsModem_Ack();
}
