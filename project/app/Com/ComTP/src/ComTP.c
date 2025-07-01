#include "ComTP.h"

static ComM_Frame_t                 comtp_frame;

static ComTP_Transaction_State_t    comtp_transaction_state;
static ComTP_EventHandler_t         comtp_event;
static ComTP_buffer_size_t          comtp_remaining_bytes;
static uint8_t                      *comtp_buffer_ptr;

/**
 * PRIVATE FUNCTIONS
 */

/**
 * Whenever something wrong happens during the transaction and the transaction needs to be aborted.
 */
static void ComTP_transactionCancelled( void );

/**
 * PUBLIC FUNCTIONS
 */

void ComTP_Init(void){
    comtp_frame.ID.id_reg = 0U;
    comtp_frame.data = 0x00U;

    comtp_transaction_state = COMTP_TRANSACTION_STATE_READY;
    comtp_event = 0x00U;
    comtp_remaining_bytes = 0U;
    comtp_buffer_ptr = 0x00U;
}

ComTP_Transaction_State_t ComTP_receiveState( void ){
    return comtp_transaction_state;
}

ComTP_Status_t ComTP_receiveAsync( void* buffer, ComTP_buffer_size_t size, ComTP_EventHandler_t callback ){
    if( ( comtp_transaction_state != COMTP_TRANSACTION_STATE_READY ) || ( buffer == 0x00 ) || ( size <= COMTP_CFG_MAX_BYTES_PER_FRAME ) ){
        return COMTP_NOT_OK;
    }

    comtp_transaction_state = COMTP_TRANSACTION_STATE_SETUP;
    comtp_buffer_ptr = buffer;
    comtp_remaining_bytes = size;
    comtp_event = callback;

    return COMTP_OK;
}

void ComTP_CancelReceive( void ){
    /// Reset state machine
    ComTP_Init();
}

ComTP_Status_t ComTP_transmitSync( void* buffer, ComTP_buffer_size_t size ){
    uint8_t* data_ptr = (uint8_t*) buffer;
    uint8_t  mini_com_buffer = 0U; /// This byte is used by the TP to setup the number of bytes to be transmitted
    ComTP_buffer_size_t frame_last_bytes = 0U;

    if( (buffer == 0x00U) || (size > COMTP_CFG_MAX_BYTES_ALLOWED ) || ( size <= COMTP_CFG_MAX_BYTES_PER_FRAME ) ){
        /**
         * Not valid transaction:
         *  - buffer = NULL ptr.
         *  - size greater than max allowed bytes.
         *  - size less or equal to the max bytes on frame (this transaction is not necessary, the data can be sent on a single frame without a TP transaction).
         */
        return COMTP_NOT_OK;
    }

    {
        /**
         * Setup a TP transaction sending the expected
         * number of bytes to the client.
         * */
        comtp_frame.ID.id = COMTP_CFG_SERVICE_ID_SETUP;
        comtp_frame.ID.len = 1u;
        mini_com_buffer = size;
        comtp_frame.data = &mini_com_buffer;

        if( COMM_NOT_OK == ComM_Send( &comtp_frame, COMM_FRAME_CONFIRM_FALSE ) ){
            return COMTP_NOT_OK;
        }

        frame_last_bytes = size % COMTP_CFG_MAX_BYTES_PER_FRAME;
        size -= frame_last_bytes;/// Align the size to the max number of bytes per frame
    }

    {
        /**
         * Send every chunk using the entire frame
         * size on every transaction.
         * */
        comtp_frame.ID.id = COMTP_CFG_SERVICE_ID_DATA;
        comtp_frame.ID.len = COMTP_CFG_MAX_BYTES_PER_FRAME;
        while( size > 0 ){
            /// Send frames with full size
            comtp_frame.data = data_ptr;

            if( COMM_NOT_OK == ComM_Send( &comtp_frame, COMM_FRAME_CONFIRM_FALSE ) ){
                return COMTP_NOT_OK;
            }

            data_ptr += COMTP_CFG_MAX_BYTES_PER_FRAME; /// Point to the next byte aligned to the full size of the frame.
            size -= COMTP_CFG_MAX_BYTES_PER_FRAME; /// Decrement by the total size of a full frame.
        }

        /**
         * Send the last comtp_frame to complete the transaction
         * */
        if( frame_last_bytes > 0 ){
            comtp_frame.ID.len = frame_last_bytes;
            comtp_frame.data = data_ptr;

            if( COMM_NOT_OK == ComM_Send( &comtp_frame, COMM_FRAME_CONFIRM_FALSE ) ){
                return COMTP_NOT_OK;
            }
        }   
    }

    /**
     * If reached, transaction was succeded
     * */
    /// Reset state machine
    ComTP_Init();
    return COMTP_OK;
}

void ComTP_transactionCancelled( void ){
    ComM_SendNack();

    if( comtp_event ){
        comtp_event( COMTP_TRANSACTION_STATE_DONE_NOT_OK );
    }

    /// Reset state machine
    ComTP_Init();
}

void ComTP_ComM_Service_SetUp( ComM_Frame_t* frame){
    if( comtp_transaction_state != COMTP_TRANSACTION_STATE_SETUP ){
        /**
         * This frame was not expected.
         */
        return ComTP_transactionCancelled();
    }

    if( (frame == 0x00) || ( frame->data == 0x00 ) || (frame->ID.len < 1U) ){
        /**
         * This frame is not valid:
         *  - frame = NULL ptr
         *  - frame->data = NULL ptr
         *  - size( frame->data ) < 1 when expected 1
         */
        return ComTP_transactionCancelled();
    }

    if( comtp_remaining_bytes != frame->data[0U] ){
        /**
         * Transaction setup doesn't match with expected transaction.
         */
        return ComTP_transactionCancelled();
    }

    /// Now switch to frame reception mode.
    comtp_transaction_state = COMTP_TRANSACTION_STATE_DATA;

    ComM_SendAck(); /// Confirm correct reception
}

void ComTP_ComM_Service_Data( ComM_Frame_t* frame){
    ComTP_buffer_size_t expected_frame_bytes;
    uint8_t *byte_ptr;

    if( comtp_transaction_state != COMTP_TRANSACTION_STATE_DATA ){
        /**
         * This frame was not expected.
         */
        return ComTP_transactionCancelled();
    }

    if( (frame == 0x00) || ( frame->data == 0x00 ) ){
        /**
         * This frame is not valid:
         *  - frame = NULL ptr
         *  - frame->data = NULL ptr
         */
        return ComTP_transactionCancelled();
    }

    if( comtp_remaining_bytes >= COMTP_CFG_MAX_BYTES_PER_FRAME ){
        expected_frame_bytes = COMTP_CFG_MAX_BYTES_PER_FRAME;
    }else{
        expected_frame_bytes = comtp_remaining_bytes;
    }

    comtp_remaining_bytes -= expected_frame_bytes;

    if( frame->ID.len != expected_frame_bytes ){
        return ComTP_transactionCancelled();
    }

    /// Confirm reception
    ComM_SendAck();

    byte_ptr = frame->data;
    while( expected_frame_bytes > 0U ){
        /**
         * Copy over all bytes to the service buffer.
         */
        *comtp_buffer_ptr = *byte_ptr;
        comtp_buffer_ptr++;
        byte_ptr++;
        expected_frame_bytes--;
    }

    if( comtp_remaining_bytes == 0U ){
        /**
         * If transaction is complete call the service callback.
         */
        if( comtp_event ){
            comtp_event( COMTP_TRANSACTION_STATE_DONE_OK );
        }
        /// Reset state machine
        ComTP_Init();
    }else{
        /**
         * If transaction is still pending, just continue.
         */
        comtp_transaction_state = COMTP_TRANSACTION_STATE_DATA;
    }
}
