#ifndef _UART_INT_H
#define	_UART_INT_H

	#include "UART_types.h"

    extern volatile uart_byte               *pTx;
    extern volatile uart_byte               *pRx;
    extern volatile uart_buffer_size_t      iTx;
    extern volatile uart_buffer_size_t      iRx;
    extern volatile uart_status_t			status;

	// Place the following macro at the correct interrupt vector
	
    #define UART_ISR() {\
                             if(PIR1bits.RC1IF){\
                                if( iRx ){\
                                    *pRx = RCREG;\
                                    pRx++;\
                                    iRx--;\
                                }else{\
                                    status &= TX_MASK;\
                                    PIE1bits.RCIE = 0;\
                                }\
                             }\
                             if(PIR1bits.TXIF){\
                                if( iTx ){\
                                    TXREG = *pTx;\
                                    pTx++;\
                                    iTx--;\
                                }else{\
                                    status &= RX_MASK;\
                                    PIE1bits.TX1IE = 0;\
                                }\
                            }\
			}
#endif
