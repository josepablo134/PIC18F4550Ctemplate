#ifndef _UART_TYPES_H
#define	_UART_TYPES_H

	#include "UART_include.h"

    #define UART_ISR_PRIORITY_LOW   0U
    #define UART_ISR_PRIORITY_HIGH  1U

    #define UART_CFG_ISR_PRIORITY   UART_ISR_PRIORITY_LOW

    #define TX_MASK             0x0F
    #define TX_BUSY             0x01
    #define TX_CANCEL           0x02
    #define RX_MASK             0xF0
    #define RX_BUSY             0x10
    #define RX_CANCEL           0x20
    #define DEFAULT_BAUD        115200

    typedef uint8_t             uart_status_t;
	typedef uint16_t            uart_buffer_size_t;
    
#endif
