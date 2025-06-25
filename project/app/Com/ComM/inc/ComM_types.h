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
    }ComM_FrameId_t;
    
    typedef struct ComM_Frame_t{
        ComM_FrameId_t      ID;
        uint8_t*            data;
    }ComM_Frame_t;

    typedef enum ComM_Status_t{
        COMM_OK=0U,
        COMM_NOT_OK=1U,
    }ComM_Status_t;

    typedef enum ComM_LockStatus_t{
        COMM_UNLOCKED=0U,
        COMM_LOCKED=1U
    }ComM_LockStatus_t;

    typedef uint8_t ComM_Event_id_t;
    
    typedef void (*ComM_EventHandler_t)(ComM_Frame_t *);

    typedef uint16_t ComM_timeout_t;

#endif
