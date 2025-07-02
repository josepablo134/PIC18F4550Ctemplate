#include "xModem.h"
#include "xModem_internal.h"

/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/

static xmodem_std_status_t xModem_getch( uint8_t *byte, xmodem_bool_t timeout_enabled );

/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/

xmodem_std_status_t xModem_Send( xmodem_packet_t* packet, xmodem_bool_t timeout_enabled ){
    uint8_t *pointer;
    uint8_t counter;
    uint8_t crc_val = XMODEM_CFG_CRC8_INIT_VAL;

    ASSERT( packet != NULL );
    ASSERT( packet->payload != NULL );
    ASSERT( packet->length <= XMODEM_CFG_MAX_BUFFER_SIZE );

    pointer = packet->payload;
    counter = packet->length + 1U;
    UART_putch( XMODEM_CHAR_SOH );
    UART_putch( packet->length );
    UART_putch( ~packet->length );
    while( counter > 0U ){
        UART_putch( *pointer );
        crc_val ^= *pointer;
        pointer++;
    }
    UART_putch( crc_val );

    return xModem_waitAck( timeout_enabled );
}

xmodem_std_status_t xModem_Receive( xmodem_packet_t* packet, xmodem_bool_t timeout_enabled ){
    uint8_t *pointer;
    uint8_t counter;
    uint8_t byte;
    uint8_t crc_val = XMODEM_CFG_CRC8_INIT_VAL;

    ASSERT( packet != NULL );
    ASSERT( packet->payload != NULL );

    /// Prepare pointer to collect the packet
    pointer = packet->payload;

    /// Expect a SOH
    if( ( xModem_getch( &byte, timeout_enabled ) != XMODEM_OK ) || ( byte != XMODEM_CFG_CHAR_SOH ) ){
        xModem_Nack();
        return XMODEM_NOT_OK;
    }

    /// Expect the size of packet
    if( ( xModem_getch( &byte, timeout_enabled ) != XMODEM_OK ) || ( byte > XMODEM_CFG_MAX_BUFFER_SIZE ) ){
        xModem_Nack();
        return XMODEM_NOT_OK;
    }

    /// Save the packet size
    counter = byte;
    
    /// Expect the 1st complement of the size
    if( ( xModem_getch( &byte, timeout_enabled ) != XMODEM_OK ) || ( byte != ~counter ) ){
        xModem_Nack();
        return XMODEM_NOT_OK;
    }

    /// Receive all bytes and calculate the CRC on the go
    counter = packet->length + 1U;
    while( counter > 0U ){
        if( ( xModem_getch( pointer, timeout_enabled ) != XMODEM_OK ) ){
            xModem_Nack();
            return XMODEM_NOT_OK;
        }
        crc_val ^= *pointer;
        pointer++;
    }

    /// Compare the CRC
    if( ( xModem_getch( &byte, timeout_enabled ) != XMODEM_OK ) || ( byte != crc_val ) ){
        xModem_Nack();
        return XMODEM_NOT_OK;
    }

    /// All good!
    xModem_Ack();
    return XMODEM_OK;
}

xmodem_std_status_t xModem_waitAck( xmodem_bool_t timeout_enabled ){
    uint8_t retries;
    uint8_t byte;

    if( timeout_enabled == XMODEM_FALSE ){
        for(retries=6U; retries>0U; retries--){
            if( XMODEM_CFG_CHAR_ACK == UART_getch() ){
                return XMODEM_OK;
            }
        }
        return XMODEM_NOT_OK;
    }

    /**
     * Wait for the char only for the specified timeout
     */
    Scheduler_SetTimeout( XMODEM_CFG_TIMEOUT_MS );
    Scheduler_StartTimer();
    byte = 0U;
    retries = 6U;
    UART_SetPollingReceive( &byte, 1U );
    while( ( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ) && ( retries > 0U ) ){
        if( !( UART_PollReceive() & RX_BUSY ) ){
            if( byte == XMODEM_CFG_CHAR_ACK ){
                break;
            }else{
                retries--;
                UART_SetPollingReceive( &byte, 1U );
            }
        }
    }
    Scheduler_StopTimer();
    UART_CancelReceive();
    return ( XMODEM_CFG_CHAR_ACK == byte ) ? XMODEM_OK: XMODEM_NOT_OK;
}

void xModem_Ack( void ){
    UART_putch( XMODEM_CHAR_ACK );
}

void xModem_Nack( void ){
    UART_putch( XMODEM_CHAR_NACK );
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/

static xmodem_std_status_t xModem_getch( uint8_t *byte, xmodem_bool_t timeout_enabled ){
    ASSERT( byte != NULL );

    if( timeout_enabled == XMODEM_FALSE ){
        // Block until char is received
        *byte = UART_getch();
        return XMODEM_OK;
    }
    /**
     * Wait for the char only for the specified timeout
     */
    xmodem_bool_t received = XMODEM_FALSE;
    Scheduler_SetTimeout( XMODEM_CFG_TIMEOUT_MS );
    Scheduler_StartTimer();
    *byte = 0U;
    UART_SetPollingReceive( byte, 1U );
    /** While timeout is not reached and continue flag is enabled */
    while( ( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ) && ( received == XMODEM_FALSE ) ){
        if( ! ( UART_PollReceive() & RX_BUSY ) ){
            received = XMODEM_TRUE;
            break;
        }
    }
    Scheduler_StopTimer();
    return received == XMODEM_TRUE? XMODEM_OK: XMODEM_NOT_OK;
}
