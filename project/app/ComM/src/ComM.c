#include "ComM.h"
#include <stdio.h>
#include <string.h>

#define COMM_UART_WAIT_UNTIL_READY_TO_SEND() { while( UART_Status() & TX_BUSY ){} }

const uart_byte COMM_FRAME_TYPE_ACK = 'A';
const uart_byte COMM_FRAME_TYPE_NACK = 'N';
const uart_byte COMM_FRAME_TYPE_DATA = 'D';

/** Frame header (1 byte) + ID (1 byte) + PAYLOAD + CRC8 (1 byte) */
static uint8_t _comm_buffer[ 3U + COMM_CFG_PAYLOAD_DATA_LEN ];

static ComM_LockStatus_t comm_status = COMM_UNLOCKED;

void ComM_Init(void){
    UART_Init();
}

void ComM_Open(void){
    UART_Open( COMM_CFG_UART_BAUDRATE );
}

void ComM_Lock(void){
    if( !comm_status ){ comm_status = COMM_UNLOCKED; }
}

void ComM_Unlock(void){
    if( comm_status ){ comm_status = COMM_UNLOCKED; }
}

ComM_LockStatus_t ComM_getLockStatus(void){
    return comm_status;
}

ComM_Status_t ComM_Send(const ComM_Payload_t* payload){
    uint8_t frame_len = 0U;
    uint8_t crc_val = COMM_CFG_CRC8_INIT_VAL;
    ComM_Status_t retVal = COMM_NOT_OK;

    if( ( comm_status == COMM_LOCKED ) || ( payload == NULL ) || ( payload->data == NULL ) || ( payload->ID.len > COMM_CFG_PAYLOAD_DATA_LEN ) || ( UART_Status() & TX_BUSY ) ){
        return retVal;
    }

    _comm_buffer[0U] = COMM_FRAME_TYPE_DATA; /** Data Frame Type */

    frame_len = payload->ID.len;
    _comm_buffer[1U] = payload->ID.id_reg;

    /// Copy content to bus buffer and calculate the CRC
    for( uint8_t index=0; index < frame_len; index++ ){
        crc_val ^= payload->data[ index ];
        _comm_buffer[ index + 2U ] = payload->data[ index ];
    }

    _comm_buffer[ 2U + frame_len ] = crc_val;

    /// Send Frame header (1byte) + ID (1byte) + payload + CRC8 (1byte)
    UART_TransmitSync( _comm_buffer, frame_len + 3U );

    retVal = COMM_OK;

    return retVal;
}

ComM_Status_t ComM_Receive( ComM_Payload_t* payload ){
    ComM_Status_t retVal = COMM_NOT_OK;
    if( comm_status == COMM_LOCKED ){
        retVal = COMM_NOT_OK;
    }
    return retVal;
}

ComM_Status_t ComM_SendAck(void){
    ComM_Status_t retVal = COMM_NOT_OK;

    if( ( comm_status == COMM_LOCKED ) || ( UART_Status() & TX_BUSY ) ){
        return retVal;
    }

    UART_TransmitSync( (const uart_byte*) &COMM_FRAME_TYPE_ACK , 1U );

    retVal = COMM_OK;
    return retVal;
}

ComM_Status_t ComM_SendNack(void){
    ComM_Status_t retVal = COMM_NOT_OK;

    if( ( comm_status == COMM_LOCKED ) || ( UART_Status() & TX_BUSY ) ){
        return retVal;
    }

    UART_TransmitSync( (const uart_byte*) &COMM_FRAME_TYPE_NACK, 1U );

    retVal = COMM_OK;
    return retVal;
}

ComM_Status_t ComM_WaitAck( ComM_timeout_t counter ){
    uint8_t continue_loop;
    ComM_Status_t retVal = COMM_NOT_OK;

    if( ( comm_status == COMM_LOCKED ) || ( UART_Status() & RX_BUSY ) ){
        return retVal;
    }

    _comm_buffer[ 0U ] = 0x00;
    UART_SetPollingReceive( _comm_buffer, 1U );
    continue_loop = 1U;
    while( ( counter > 1U ) && ( continue_loop ) ){
        if( ! ( UART_PollReceive( 1U ) & RX_BUSY ) ){ /// Reception completed
            switch ( _comm_buffer[0U] )
            {
                case COMM_FRAME_TYPE_NACK:
                    continue_loop = 0U;
                    retVal = COMM_NOT_OK;
                    continue_loop = 0U;
                    break;
                case COMM_FRAME_TYPE_ACK:
                    retVal = COMM_OK;
                    continue_loop = 0U;
                    break;
                default:
                    UART_SetPollingReceive( _comm_buffer, 1U );
                    break;
            }
        }
    }

    return retVal;
}
