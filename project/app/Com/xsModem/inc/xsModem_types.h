#ifndef _XSMODEM_TYPES_H_
#define _XSMODEM_TYPES_H_

    #include <stdint.h>

    typedef struct xsmodem_packet_t{
        uint8_t*    payload;
        uint8_t     length;
    }xsmodem_packet_t;

    typedef enum xsmodem_std_status_t {
        XSMODEM_OK=0U,
        XSMODEM_NOT_OK
    } xsmodem_std_status_t;

    typedef enum xsmodem_bool_t {
        XSMODEM_FALSE=0U,
        XSMODEM_TRUE
    } xsmodem_bool_t ;

#endif
