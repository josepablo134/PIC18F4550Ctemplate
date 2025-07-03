#include "xsModem.h"
#include "xsModem_internal.h"

/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/

static xsmodem_std_status_t xsModem_getch( uint8_t *byte, xsmodem_bool_t timeout_enabled );

static xsmodem_std_status_t xsModem_wait_ch( const uint8_t expected_byte, xsmodem_bool_t timeout_enabled );

/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/

xsmodem_std_status_t xsModem_Transmit( xsmodem_packet_t* packet, xsmodem_bool_t timeout_enabled ){
    uint8_t *pointer;
    uint8_t counter;
    uint8_t crc_val = XSMODEM_CFG_CRC8_INIT_VAL;

    ASSERT( packet != NULL );
    ASSERT( packet->payload != NULL );
    ASSERT( packet->length <= XSMODEM_CFG_MAX_BUFFER_SIZE );

    pointer = packet->payload;
    counter = packet->length;

    /// Wait for the ready signal or a timeout
    if( XSMODEM_NOT_OK == xsModem_waitNack( timeout_enabled ) ){
        return XSMODEM_NOT_OK;
    }

    UART_putch( XSMODEM_CHAR_SOH );
    UART_putch( packet->length );
    UART_putch( ~packet->length );
    while( 0U < counter-- ){
        UART_putch( *pointer );
        crc_val ^= *pointer;
        pointer++;
    }
    UART_putch( crc_val );

    /// Wait for confirmation signal
    return xsModem_waitAck( timeout_enabled );
}

xsmodem_std_status_t xsModem_Receive( xsmodem_packet_t* packet, xsmodem_bool_t timeout_enabled ){
    uint8_t *pointer;
    uint8_t counter;
    uint8_t byte;
    uint8_t crc_val = XSMODEM_CFG_CRC8_INIT_VAL;

    ASSERT( packet != NULL );
    ASSERT( packet->payload != NULL );

    /// Prepare pointer to collect the packet
    pointer = packet->payload;

    /// Send the ready signal
    xsModem_Nack();

    /// Expect a SOH
    if( ( xsModem_getch( &byte, timeout_enabled ) == XSMODEM_NOT_OK ) || ( byte != XSMODEM_CFG_CHAR_SOH ) ){
        xsModem_Nack();
        return XSMODEM_NOT_OK;
    }

    /// Expect the size of packet
    if( ( xsModem_getch( &byte, timeout_enabled ) == XSMODEM_NOT_OK ) || ( byte > XSMODEM_CFG_MAX_BUFFER_SIZE ) ){
        xsModem_Nack();
        return XSMODEM_NOT_OK;
    }

    /// Save the packet size
    counter = byte;

    /// Expect the 1st complement of the size
    if( ( xsModem_getch( &byte, timeout_enabled ) == XSMODEM_NOT_OK ) || ( byte != (uint8_t) ~counter ) ){
        xsModem_Nack();
        return XSMODEM_NOT_OK;
    }

    /// Receive all bytes and calculate the CRC on the go
    counter = packet->length;
    while( 0U < counter-- ){
        if( ( xsModem_getch( pointer, timeout_enabled ) == XSMODEM_NOT_OK ) ){
            xsModem_Nack();
            return XSMODEM_NOT_OK;
        }
        crc_val ^= *pointer;
        pointer++;
    }

    /// Compare the CRC
    if( ( xsModem_getch( &byte, timeout_enabled ) == XSMODEM_NOT_OK ) || ( byte != crc_val ) ){
        xsModem_Nack();
        return XSMODEM_NOT_OK;
    }

    /// All good!
    xsModem_Ack();
    return XSMODEM_OK;
}

xsmodem_std_status_t xsModem_waitAck( xsmodem_bool_t timeout_enabled ){
    return xsModem_wait_ch( XSMODEM_CHAR_ACK, timeout_enabled );
}

xsmodem_std_status_t xsModem_waitNack( xsmodem_bool_t timeout_enabled ){
    return xsModem_wait_ch( XSMODEM_CHAR_NACK, timeout_enabled );
}

void xsModem_Ack( void ){
    UART_putch( XSMODEM_CHAR_ACK );
}

void xsModem_Nack( void ){
    UART_putch( XSMODEM_CHAR_NACK );
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/

static xsmodem_std_status_t xsModem_getch( uint8_t *byte, xsmodem_bool_t timeout_enabled ){
    ASSERT( byte != NULL );

    if( timeout_enabled == XSMODEM_FALSE ){
        // Block until char is received
        *byte = UART_getch();
        return XSMODEM_OK;
    }
    /**
     * Wait for the char only for the specified timeout
     */
    xsmodem_bool_t received = XSMODEM_FALSE;
    Scheduler_SetTimeout( XSMODEM_CFG_TIMEOUT_MS );
    Scheduler_StartTimer();
    *byte = 0U;
    UART_SetPollingReceive( byte, 1U );
    /** While timeout is not reached and continue flag is enabled */
    while( ( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ) && ( received == XSMODEM_FALSE ) ){
        if( ! ( UART_PollReceive() & RX_BUSY ) ){
            received = XSMODEM_TRUE;
            break;
        }
    }
    Scheduler_StopTimer();
    return received == XSMODEM_TRUE? XSMODEM_OK: XSMODEM_NOT_OK;
}

static xsmodem_std_status_t xsModem_wait_ch( const uint8_t expected_byte, xsmodem_bool_t timeout_enabled ){
    uint8_t retries;
    uint8_t byte;

    if( timeout_enabled == XSMODEM_FALSE ){
        for(retries=6U; retries>0U; retries--){
            if( expected_byte == UART_getch() ){
                return XSMODEM_OK;
            }
        }
        return XSMODEM_NOT_OK;
    }

    /**
     * Wait for the char only for the specified timeout
     */
    Scheduler_SetTimeout( XSMODEM_CFG_TIMEOUT_MS );
    Scheduler_StartTimer();
    byte = 0U;
    retries = 6U;
    UART_SetPollingReceive( &byte, 1U );
    while( ( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ) && ( retries > 0U ) ){
        if( !( UART_PollReceive() & RX_BUSY ) ){
            if( byte == expected_byte ){
                break;
            }else{
                retries--;
                UART_SetPollingReceive( &byte, 1U );
            }
        }
    }
    Scheduler_StopTimer();
    UART_CancelReceive();
    return ( expected_byte == byte ) ? XSMODEM_OK: XSMODEM_NOT_OK;
}
