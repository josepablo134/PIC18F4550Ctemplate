#ifndef _COMM_TYPES_H_
#define _COMM_TYPES_H_

    #include <stdint.h>

    typedef union
    {
        struct
        {
            unsigned len :4;
            unsigned id  :4;
        };
        uint8_t           id_reg;
    }ComM_Payload_id_t;
    
    typedef struct ComM_Payload_t{
        ComM_Payload_id_t   ID;
        uint8_t*            data;
    }ComM_Payload_t;

    typedef enum ComM_Status_t{
        COMM_OK=0U,
        COMM_NOT_OK=1U,
    }ComM_Status_t;

    typedef enum ComM_LockStatus_t{
        COMM_UNLOCKED=0U,
        COMM_LOCKED=1U
    }ComM_LockStatus_t;

    typedef uint32_t ComM_timeout_t;

#endif
