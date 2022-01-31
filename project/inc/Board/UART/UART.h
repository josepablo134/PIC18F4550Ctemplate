#ifndef _UART_H
#define	_UART_H

    #include "UART_include.h"
    #include "UART_internal.h"
	#include "UART_types.h"

    /// Inicializar el puerto serial
    extern void UART_init(void);

    /// Inicializar el puerto serial
    extern void UART_open(unsigned long);

    /// Comenzar una transmision
    extern uart_status_t UART_TransmitAsync(const void* , uart_buffer_size_t);

	extern uart_status_t UART_TransmitSync(const void* , uart_buffer_size_t);

    extern uart_status_t UART_CancelTransmit(void);

    /// Comenzar una recepcion
    extern uart_status_t UART_Receive(void* , uart_buffer_size_t);

    extern uart_status_t UART_CancelReceive(void);

    /// Solicitar el estado de la transferencia
    extern uart_status_t UART_Status(void);
    
#endif
