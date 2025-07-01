#ifndef _COMTP_CFG_H_
#define _COMTP_CFG_H_

    #include "ComTP_types.h"

    #define COMTP_CFG_MAX_BYTES_ALLOWED                 (32U)
    #define COMTP_CFG_MAX_BYTES_PER_FRAME               (COMM_CFG_PAYLOAD_DATA_LEN)
    #define COMTP_CFG_MAX_FRAMES_ALLOWED                (COMTP_CFG_MAX_BYTES_ALLOWED / COMTP_CFG_MAX_BYTES_PER_FRAME)

    #define COMTP_CFG_SERVICE_ID_DATA                   (0x01)
    #define COMTP_CFG_SERVICE_ID_SETUP                  (0x02)

#endif
