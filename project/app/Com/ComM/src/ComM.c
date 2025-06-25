#include "ComM.h"
#include <stdio.h>
#include <string.h>

#define COMM_UART_WAIT_UNTIL_READY_TO_SEND() { while( UART_Status() & TX_BUSY ){} }

const uart_byte COMM_FRAME_TYPE_ACK = 'A';
const uart_byte COMM_FRAME_TYPE_NACK = 'N';
const uart_byte COMM_FRAME_TYPE_DATA = 'D';

/** Frame header (1 byte) + ID (1 byte) + PAYLOAD + CRC8 (1 byte) */
static uint8_t _comm_buffer[ 3U + COMM_CFG_PAYLOAD_DATA_LEN ];

static ComM_EventHandler_t event_list[ COMM_CFG_MAX_NUM_OF_EVENTS ];

static ComM_LockStatus_t comm_status = COMM_UNLOCKED;


/**
 * PRIVATE FUNCTIONS
 */

/** 
 * Block until a byte is received or timeout is reached
 */
static ComM_Status_t ComM_receiveByteTimeout( ComM_timeout_t timeout_ms, uint8_t *byte );

/**
 * Block until a byte is received
 */
static uint8_t ComM_receiveByte( void );

/**
 * Block until service returns
 */
static inline void ComM_invokeService( ComM_Frame_t *frame );


void ComM_Init(void){
    UART_Init();

    /// Clear the event list
    for( ComM_Event_id_t id=0U; id<COMM_CFG_MAX_NUM_OF_EVENTS; id++ ){
        event_list[ id ] = 0x00U;
    }
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

ComM_Status_t ComM_Send(const ComM_Frame_t* payload){
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

static ComM_Status_t ComM_receiveByteTimeout( ComM_timeout_t timeout_ms, uint8_t *byte ){
    ComM_Status_t retVal = COMM_NOT_OK;
    Scheduler_SetTimeout( timeout_ms );
    Scheduler_StartTimer();

    UART_SetPollingReceive( byte, 1U );
    while( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ){
        if( ! ( UART_PollReceive() & RX_BUSY ) ){ /// Reception completed
            retVal = COMM_OK;
            break;
        }
    }

    Scheduler_StopTimer();
    UART_CancelReceive();/// Just in case timeout was reached.

    return retVal;
}

static uint8_t ComM_receiveByte( void ){
    uint8_t byte;
    UART_SetPollingReceive( &byte, 1U );
    while( UART_PollReceive() & RX_BUSY ){}
    return byte;
}

ComM_Status_t ComM_WaitAck( ComM_timeout_t timeout_ms ){
    uint8_t continue_loop;
    ComM_Status_t retVal = COMM_NOT_OK;

    if( ( comm_status == COMM_LOCKED ) || ( UART_Status() & RX_BUSY ) ){
        return retVal;
    }

    _comm_buffer[ 0U ] = 0x00;

    Scheduler_SetTimeout( timeout_ms );
    Scheduler_StartTimer();

    UART_SetPollingReceive( _comm_buffer, 1U );

    /** While timeout is not reached and continue flag is enabled */
    while( ( Scheduler_TimeoutReached() == SCHEDULER_TIMEOUT_NOT_REACHED ) && ( continue_loop ) ){
        if( ! ( UART_PollReceive() & RX_BUSY ) ){ /// New byte received
            switch ( _comm_buffer[0U] )
            {
                case COMM_FRAME_TYPE_NACK:
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

    Scheduler_StopTimer();
    return retVal;
}

ComM_Status_t ComM_SetEventHandler( ComM_Event_id_t id, ComM_EventHandler_t callback){
    if( id >= COMM_CFG_MAX_NUM_OF_EVENTS ){
        return COMM_NOT_OK;
    }
    event_list[ id ] = callback;
    return COMM_OK;
}

static inline void ComM_invokeService( ComM_Frame_t *frame ){
    ComM_Event_id_t id = frame->ID.id;

    if( id > COMM_CFG_MAX_NUM_OF_EVENTS ){
        return;
    }

    /// Invoke the service event
    return event_list[ id ]( frame );
}

void ComM_mainFunction( void ){
    ComM_Frame_t frame;
    uint8_t *buff_ptr;
    uint8_t counter;
    uint8_t crc_val = COMM_CFG_CRC8_INIT_VAL;

    frame.data = _comm_buffer;
    buff_ptr = _comm_buffer;

    for(;;){
        switch ( ComM_receiveByte() )
        {
            case COMM_FRAME_TYPE_DATA:
                {
                    frame.ID.id_reg = ComM_receiveByte();

                    if( frame.ID.len > 8U ){
                        /// Reject received frame
                        ComM_SendNack();
                        continue;
                    }

                    /** Receive every data byte and calculate the CRC on the go */
                    buff_ptr = _comm_buffer;
                    for( counter=0U; counter < frame.ID.len; counter ++ ){
                        *buff_ptr = ComM_receiveByte();
                        crc_val ^= *buff_ptr;
                        buff_ptr++;
                    }

                    /**
                     * Validate the CRC with the end of frame, if valid invoke the service.
                     * */
                    *buff_ptr = ComM_receiveByte();// Get the CRC byte
                    if( *buff_ptr == crc_val ){
                        ComM_invokeService( &frame );
                    }else{
                        /// Reject received frame
                        ComM_SendNack();
                    }
                }
                break;
            case COMM_FRAME_TYPE_NACK:
            case COMM_FRAME_TYPE_ACK:
            default:
                break;
        }
    }    
}
