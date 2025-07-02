#ifndef _UART_CFG_H
#define	_UART_CFG_H

	#include "UART_types.h"

    #define UART_CFG_ISR_PRIORITY   UART_ISR_PRIORITY_LOW
    #define DEFAULT_BAUDRATE        115200U

    /**
     * 
     * The following are the UART parameters for a 115200U bauds config
     *  
    */
    #define UART_CFG_STATIC_CONFIG_ENABLED
    #define UART_CFG_STATIC_CONFIG_BAUDCON      (0x4A)
    #define UART_CFG_STATIC_CONFIG_SPBRG        (0x19)
    #define UART_CFG_STATIC_CONFIG_SPBRGH       (0x00)
    #define UART_CFG_STATIC_CONFIG_TXSTA        (0x20)
    #define UART_CFG_STATIC_CONFIG_RCSTA        (0x90)

#endif
