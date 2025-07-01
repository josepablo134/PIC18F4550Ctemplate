#ifndef _COMTP_TYPES_H_
#define _COMTP_TYPES_H_

    #include <stdint.h>

    typedef uint8_t ComTP_buffer_size_t;

    typedef enum ComTP_Status_t{
        COMTP_OK=0U,
        COMTP_NOT_OK=1U,
    }ComTP_Status_t;

    typedef enum ComTP_Transaction_State_t{
        COMTP_TRANSACTION_STATE_READY=0U,
        COMTP_TRANSACTION_STATE_SETUP,
        COMTP_TRANSACTION_STATE_DATA,
        COMTP_TRANSACTION_STATE_DONE_OK,
        COMTP_TRANSACTION_STATE_DONE_NOT_OK,
    }ComTP_Transaction_State_t;

    typedef void (*ComTP_EventHandler_t)(ComTP_Transaction_State_t);

#endif
