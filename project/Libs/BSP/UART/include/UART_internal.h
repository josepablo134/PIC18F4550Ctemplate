#ifndef _UART_INT_H
#define	_UART_INT_H

	#include "project_macros.h"
	#include "UART_types.h"

    extern volatile uart_byte               *pTx;
    extern volatile uart_byte               *pRx;
    extern volatile uart_buffer_size_t      iTx;
    extern volatile uart_buffer_size_t      iRx;
    extern volatile uart_status_t			status;

    /** When reaching this ISR is because there is something pending to receive! (iRx > 1) */
    #define UART_RX_ISR() {\
        if(PIR1bits.RC1IF){\
            *pRx = RCREG;\
            iRx--;\
            if( iRx ){\
                pRx++;\
            }else{\
                status &= ~RX_MASK;\
                PIE1bits.RCIE = 0;\
            }\
        }\
    }
	
    /** When reaching this ISR is because there is something pending to sent! (iTx > 1) */
    #define UART_TX_ISR() {\
        if(PIR1bits.TXIF){\
            TXREG = *pTx;\
            iTx--;\
            if( iTx ){\
                pTx++;\
            }else{\
                status &= ~TX_MASK;\
                PIE1bits.TX1IE = 0;\
            }\
        }\
    }

#endif
