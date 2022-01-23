#ifndef _UART_H
#define	_UART_H
    #include <stdint.h>
    #include "../config/board_config.h"

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

    extern volatile unsigned char          *pTx,*pRx;
    extern volatile unsigned int            iTx,iRx;
    extern volatile uint8_t                 status;
    
    /// Inicializar el puerto serial
    extern void UART_init(void);
    /// Inicializar el puerto serial
    extern void UART_open(unsigned long);
    /// Comenzar una transmision
    extern uint8_t UART_Transmit(void* , unsigned int);
    extern void UART_CancelTransmit(void);
    /// Comenzar una recepcion
    extern uint8_t UART_Receive(void* , unsigned int);
    extern void UART_CancelReceive(void);
    /// Solicitar el estado de la transferencia
    extern uint8_t UART_Status(void);
    
    #define UART_INTERRUPT  {                                           \
                             if(PIR1bits.RC1IF){                        \
                                *pRx = RCREG;                           \
                                *pRx++;                                 \
                                iRx--;                                  \
                                if( !iRx ){                             \
                                    status &= TX_MASK;                  \
                                    PIE1bits.RCIE = 0;                  \
                                }                                       \
                             }                                          \
                             if(PIR1bits.TXIF){                         \
                                iTx--;                                  \
                                if( iTx ){                              \
                                    TXREG = *pTx;                       \
                                    *pTx++;                             \
                                }else{                                  \
                                    status &= RX_MASK;                  \
                                    PIE1bits.TX1IE = 0;                 \
                                }                                       \
                            }                                           \
                            }
#endif