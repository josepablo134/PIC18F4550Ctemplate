#include "../../../inc/Board/UART/UART.h"

volatile uint8_t status;
volatile unsigned char *pTx;
volatile unsigned char *pRx;
volatile unsigned int iTx;
volatile unsigned int iRx;

    /// Inicializar el puerto serial
void UART_init(){
    status = 0x00;
    pTx = pRx = 0x00;
    iTx = iRx = 0x00;
    return;
}
void UART_open(uint32_t baud){
    uint16_t buffer;
    //Configurar pines
    //TX(C6) - RX(C7)
    PORTC = 0;LATC  = 0;TRISC = 0xC0;
    
    /// Receive Active(0) - BRG16(1) - WUE(1)
    BAUDCON = 0b00001010;
    /// BRGH(0) - TXEN(0) - SYNC(0) : Modo Asincrono
    TXSTA = 0b00000000;
    /// SPEN(0) - CREN(0)           : Modo Asincrono
    RCSTA = 0b00000000;
    
    /// Generador de baudrate automatico
    // FOSC/[64(n+1)] = Baud
    //=>    [(FOSC/Baud)/64]-1;
    //SPBRGH = 0x00;
    if( !TXSTAbits.SYNC ){
        if( !BAUDCONbits.BRG16 ){
            if( TXSTAbits.BRGH ){
                SPBRG  = (uint8_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)16U) - (uint8_t)1U;
            }else{
                SPBRG  = (uint8_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)64U)- (uint8_t)1U;                
            }
        }else{
            if( TXSTAbits.BRGH ){
                buffer = (uint16_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)4U) - (uint16_t)1U;
                SPBRG  = buffer & 0xFF;
                SPBRGH = buffer >> 8;
            }else{
                buffer = (uint16_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)16U) - (uint16_t)1U; 
                SPBRG  = buffer & 0xFF;
                SPBRGH = buffer >> 8;
            }
        }
    }else{
        if( !BAUDCONbits.BRG16 ){
                SPBRG  = (uint8_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)4) - (uint8_t)1U;
        }else{
                buffer = (uint16_t)((uint32_t)_XTAL_FREQ/(uint32_t)baud/(uint32_t)4U) - (uint16_t)1U;
                SPBRG  = buffer & 0xFF;
                SPBRGH = buffer >> 8;
        }
    }
    
    ///Activar el puerto UART
    #if (UART_CFG_ISR_PRIORITY == UART_ISR_PRIORITY_LOW)
        IPR1bits.TX1IP = 0;//Sin interrupcion, pero registrada como low
        IPR1bits.RC1IP = 0;//Sin interrupcion, pero registrada como low
    #else
        IPR1bits.TX1IP = 1;//Sin interrupcion, pero registrada como high
        IPR1bits.RC1IP = 1;//Sin interrupcion, pero registrada como high
    #endif
    TXSTAbits.TXEN = 1;//Tx Enable
    RCSTAbits.CREN = 1;//Rx Enable
    RCSTAbits.SPEN = 1;//Serial Port Enable
}
    /// Comenzar una transmision
uint8_t UART_TransmitAsync(const void* buffer, uart_buffer_size_t size){
    if( buffer && size ){ // Si la direccion o el tamanio son validos
        if( pTx ){/// Si no hay alguna transmision pendiente
            if( status & TX_BUSY ){
                return status;
            }else{
                goto SET_PTR;
            }
        }else{
            goto SET_PTR;
        }
    }else{
        return 0xFF;
    }
SET_PTR:
    pTx = (void*) buffer;
    iTx = size;
    status |= TX_BUSY;
    PIE1bits.TX1IE = 1;     //Activar sistema de interrupciones
    return 0;
}
uint8_t UART_TransmitSync(const void* buffer, uart_buffer_size_t size){
    if( buffer && size ){ // Si la direccion o el tamanio son validos
        if( pTx ){/// Si no hay alguna transmision pendiente
            if( status & TX_BUSY ){
                return status;
            }else{
                goto SEND;
            }
        }else{
            goto SEND;
        }
    }else{
        return 0xFF;
    }
SEND:
    pTx = (void*) buffer;
    iTx = size;
	// Set TX BUSY Flag
    status |= TX_BUSY;
	while( iTx ){
		if(PIR1bits.TXIF){
			iTx--;
			if( iTx ){
				TXREG = *pTx;
				*pTx++;
			}else{
				// Clear TX BUSY flag
				status &= RX_MASK;
			}
		}
	}
    return status;
}

uart_status_t UART_CancelTransmit(void){
    PIE1bits.TX1IE = 0;     //Desactivar sistema de interrupciones
    status = (status&RX_MASK) | TX_CANCEL;
    return status;
}

/// Comenzar una recepcion
uart_status_t UART_Receive(void* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Si la direccion o el tamanio son validos
        if( pRx ){/// Si no hay alguna transmision pendiente
            if( status & RX_BUSY ){
                return status;
            }else{
                goto SET_PTR;
            }
        }else{
            goto SET_PTR;
        }
    }else{
        return 0xFF;
    }
SET_PTR:
    pRx = buffer;
    iRx = size;
    status |= RX_BUSY;
    PIE1bits.RC1IE = 1;     //Activar sistema de interrupciones
    return status;
}

uart_status_t UART_CancelReceive(void){
    PIE1bits.RC1IE = 0;     //Desactivar sistema de interrupciones
    status = (status&TX_MASK) | RX_CANCEL;
    return status;
}
    /// Solicitar el estado de la transferencia
uint8_t UART_Status(void){
    return status;
}
