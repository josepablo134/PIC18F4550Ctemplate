#ifndef _XMODEM_TYPES_H_
#define _XMODEM_TYPES_H_

    #include <stdint.h>

    typedef struct xmodem_packet_t{
        uint8_t*    payload;
        uint8_t     length;
    }xmodem_packet_t;

    typedef enum xmodem_std_status_t {
        XMODEM_OK=0U,
        XMODEM_NOT_OK
    } xmodem_std_status_t;

    typedef enum xmodem_bool_t {
        XMODEM_FALSE=0U,
        XMODEM_TRUE
    } xmodem_bool_t ;

#endif
