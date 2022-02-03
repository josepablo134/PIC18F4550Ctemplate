#include "../../../inc/Board/UART/UART.h"

volatile uart_status_t status;
volatile uart_byte *pTx;
volatile uart_byte *pRx;
volatile uart_buffer_size_t iTx;
volatile uart_buffer_size_t iRx;

/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/
static void UART_internalTransmitAsync(const uart_byte* buffer,
        uart_buffer_size_t size);
static void UART_internalTransmitSync(const uart_byte* buffer,
        uart_buffer_size_t size);
static void UART_internalReceiveAsync(uart_byte* buffer,
        uart_buffer_size_t size);
static void UART_internalReceiveSync(uart_byte* buffer,
        uart_buffer_size_t size);

/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/
void UART_Init(){
    status = 0x00;
    pTx = pRx = 0x00;
    iTx = iRx = 0x00;
    return;
}

void UART_Open(uart_baudrate baud){
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

uart_status_t UART_TransmitAsync(const uart_byte* buffer, uart_buffer_size_t size){
    if( buffer && size ){// Si la direccion o el tamanio son validos
        if( pTx ){/// Si no hay alguna transmision pendiente
            if( status & TX_BUSY ){
                return status;
            }else{
                UART_internalTransmitAsync( buffer , size );
            }
        }else{
            UART_internalTransmitAsync( buffer , size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_TransmitSync(const uart_byte* buffer, uart_buffer_size_t size){
    if( buffer && size ){ // Si la direccion o el tamanio son validos
        if( pTx ){/// Si no hay alguna transmision pendiente
            if( status & TX_BUSY ){
                return status;
            }else{
                UART_internalTransmitSync( buffer, size );
            }
        }else{
            UART_internalTransmitSync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_CancelTransmit(void){
    PIE1bits.TX1IE = 0;     //Desactivar sistema de interrupciones
    status = (status&RX_MASK) | TX_CANCEL;
    return status;
}

uart_status_t UART_ReceiveAsync(uart_byte* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Si la direccion o el tamanio son validos
        if( pRx ){/// Si no hay alguna transmision pendiente
            if( status & RX_BUSY ){
                return status;
            }else{
                UART_internalReceiveAsync( buffer, size );
            }
        }else{
            UART_internalReceiveAsync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_ReceiveSync(uart_byte* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Si la direccion o el tamanio son validos
        if( pRx ){/// Si no hay alguna transmision pendiente
            if( status & RX_BUSY ){
                return status;
            }else{
                UART_internalReceiveSync( buffer, size );
            }
        }else{
            UART_internalReceiveSync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_CancelReceive(void){
    PIE1bits.RC1IE = 0;     //Desactivar sistema de interrupciones
    status = (status&TX_MASK) | RX_CANCEL;
    return status;
}

uart_status_t UART_Status(void){
    return status;
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/
static void UART_internalTransmitAsync(const uart_byte* buffer, uart_buffer_size_t size){
    pTx = (void*) buffer;
    iTx = size;
    status |= TX_BUSY;
    PIE1bits.TX1IE = 1;/// Enable TX interrupt
}

static void UART_internalTransmitSync(const uart_byte* buffer, uart_buffer_size_t size){
    pTx = (void*) buffer;
    iTx = size;
	// Set TX BUSY Flag
    status |= TX_BUSY;
	while( iTx ){
		if(PIR1bits.TXIF){
            TXREG = *pTx;
            *pTx++;
			iTx--;
		}
	}
    // Clear TX BUSY flag
    status &= RX_MASK;
}

static void UART_internalReceiveAsync(uart_byte* buffer, uart_buffer_size_t size){
    pRx = buffer;
    iRx = size;
    status |= RX_BUSY;
    PIE1bits.RC1IE = 1;/// Enable RX interrupt
}

static void UART_internalReceiveSync(uart_byte* buffer, uart_buffer_size_t size){
    pRx = buffer;
    iRx = size;
    // Set RX BUSY flag
    status |= RX_BUSY;
    while( iRx ){
        if(PIR1bits.RC1IF){
            *pRx = RCREG;
            *pRx++;
            iRx--;
        }
    }
    // Clear RX BUSY flag
    status &= TX_MASK;
}

