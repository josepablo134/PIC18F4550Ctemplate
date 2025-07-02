#ifndef _UART_H
#define	_UART_H

#ifdef _SPI_H_
    #error "This driver is not compatible with the SPI driver"
#endif
#ifdef _IIC_H_
    #error "This driver is not compatible with the IIC driver"
#endif

    #include "UART_include.h"
	#include "UART_types.h"
    #include "UART_internal.h"
    #include "UART_cfg.h"

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

    #ifdef UART_CFG_STATIC_CONFIG_ENABLED
        /**
         * @brief Initialize peripheral using REG vals from MACROS to save instructions
         * */
        extern void UART_OpenStatic(void);
    #endif

    /**
     * @brief Blocks execution until the bus is ready for transmission, then sets the byte and returns.
     * @remarks It does the action right away without taking into account any current transmission flow.
     */
    extern void UART_putch( uint8_t );

    /**
     * @brief Blocks execution until the char is received, then returns the byte.
     * @remarks It does the action right away without taking into account any current transmission flow.
     */
    extern uint8_t UART_getch( void );

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
     * @brief Set a receive request to the state machine but do not use the interrupts
     * */
    extern uart_status_t UART_SetPollingReceive(uart_byte* , uart_buffer_size_t);

    /**
     * @brief Check if receive request is completed by copying all bytes left from the UART buffer within the specified number of iterations.
     * */
    extern uart_status_t UART_PollReceive(void);

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
