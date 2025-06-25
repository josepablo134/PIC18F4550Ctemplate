#include "UART.h"

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
static void UART_internalSetPollingReceive(uart_byte* buffer,
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
    if( buffer && size ){// Check pointer and size are valid
        if( status & TX_BUSY ){
            return status;
        }else{
            UART_internalTransmitAsync( buffer , size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_TransmitSync(const uart_byte* buffer, uart_buffer_size_t size){
    if( buffer && size ){ // Check pointer and size are valid
        if( status & TX_BUSY ){
            return status;
        }else{
            UART_internalTransmitSync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_CancelTransmit(void){
    PIE1bits.TX1IE = 0;     //Disable interrupts
    status = ( status & (~TX_MASK) ); // Clear TX flags
    return status;
}

uart_status_t UART_ReceiveAsync(uart_byte* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Check pointer and size are valid
        if( status & RX_BUSY ){
            return status;
        }else{
            UART_internalReceiveAsync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_ReceiveSync(uart_byte* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Check pointer and size are valid
        if( status & RX_BUSY ){
            return status;
        }else{
            UART_internalReceiveSync( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_SetPollingReceive(uart_byte* buffer, uart_buffer_size_t size){
    if( buffer || size ){ // Check pointer and size are valid
        if( status & RX_BUSY ){
            return status;
        }else{
            UART_internalSetPollingReceive( buffer, size );
        }
    }else{
        return UART_ERROR;
    }
    return status;
}

uart_status_t UART_PollReceive(uart_timeout_t counts){
    while( counts-- ){
        UART_RX_ISR();/// Execute the same logic as the ISR
        if( !(status & RX_BUSY) ){
            /// Finish polling if status is complete
            break;
        }
    }
    return status;
}

uart_status_t UART_CancelReceive(void){
    PIE1bits.RC1IE = 0;     //Disable interrupts
    status = ( status & (~RX_MASK) );// Clear RX flags
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
    PIE1bits.TX1IE = 1U;/// Enable TX interrupt
}

static void UART_internalTransmitSync(const uart_byte* buffer, uart_buffer_size_t size){
    pTx = (void*) buffer;
    iTx = size;
    status |= TX_BUSY;
	while( iTx ){
        UART_TX_ISR();
	}
    // Clear TX BUSY flag
    status &= RX_MASK;
}

static void UART_internalReceiveAsync(uart_byte* buffer, uart_buffer_size_t size){
    pRx = buffer;
    iRx = size;
    status |= RX_BUSY;
    PIE1bits.RC1IE = 1U;/// Enable RX interrupt
}

static void UART_internalReceiveSync(uart_byte* buffer, uart_buffer_size_t size){
    pRx = buffer;
    iRx = size;
    // Set RX BUSY flag
    status |= RX_BUSY;

    while( iRx ){
        UART_RX_ISR();
    }

    // Clear RX BUSY flag
    status &= TX_MASK;
}

static void UART_internalSetPollingReceive(uart_byte* buffer, uart_buffer_size_t size){
    pRx = buffer;
    iRx = size;
    status |= RX_BUSY;
}
