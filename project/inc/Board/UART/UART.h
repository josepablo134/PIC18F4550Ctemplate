#ifndef _UART_H
#define	_UART_H

    #include "UART_include.h"
	#include "UART_types.h"

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Initialize internal state machine
     * */
    extern void UART_Init(void);

    /**
     * @brief Initialize peripheral
     * */
    extern void UART_Open(uart_baudrate);

    /**
     * @brief Send a buffer of bytes using interrupts
     * */
    extern uart_status_t UART_TransmitAsync(const uart_byte* , uart_buffer_size_t);

    /**
     * @brief Send a buffer of bytes and halt until operation finishes
     * */
	extern uart_status_t UART_TransmitSync(const uart_byte* , uart_buffer_size_t);

    /**
     * @brief Cancel an ongoing transmission
     * */
    extern uart_status_t UART_CancelTransmit(void);

    /**
     * @brief Receive bytes using interrupts
     * */
    extern uart_status_t UART_ReceiveAsync(uart_byte* , uart_buffer_size_t);

    /**
     * @brief Receive bytes and halt until operation finishes
     * */
    extern uart_status_t UART_ReceiveSync(uart_byte* , uart_buffer_size_t);
    
    /**
     * @brief Cancel an ongoing reception
     * */
    extern uart_status_t UART_CancelReceive(void);

    /**
     * @brief Request driver status (see UART bit flags)
     * */
    extern uart_status_t UART_Status(void);

#ifdef	__cplusplus
}
#endif

#endif
