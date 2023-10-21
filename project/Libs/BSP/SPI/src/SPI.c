#include "SPI.h"

volatile SPIState_t SPI_internal_state = SPI_uninit;
volatile SPI_byte_t*        __txBuffer = NULL;
volatile SPI_byte_t*        __rxBuffer = NULL;
volatile SPI_buffer_size_t  __txSize = 0U;
volatile SPI_buffer_size_t  __rxSize = 0U;

/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/

static void SPI_InternalTransactionSync( void );
static void SPI_InternalTransactionAsync( void );

/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/

void SPI_Init(void){
    /* Nothing to do here */
}

void SPI_Open(void){
    ASSERT( SPI_uninit == SPI_internal_state );

    PORTAbits.RA5 = 0U;/// SS  (Slave Select)
    PORTCbits.RC7 = 0U;/// SDO (Serial Data Output)
    PORTBbits.RB0 = 0U;/// SDI (Serial Data Input)
    PORTBbits.RB1 = 0U;/// SCK (Serial Clock)

    LATAbits.LA5 = 1U;/// SS  (Slave Select)
    LATCbits.LC7 = 0U;/// SDO (Serial Data Output)
    LATBbits.LB0 = 0U;/// SDI (Serial Data Input)
    LATBbits.LB1 = 0U;/// SCK (Serial Clock)

    TRISAbits.RA5 = 0U;/// SS  (Slave Select)
    TRISCbits.RC7 = 0U;/// SDO (Serial Data Output)
    TRISBbits.RB0 = 1U;/// SDI (Serial Data Input)
    TRISBbits.RB1 = 0U;/// SCK (Serial Clock)

    SPI_SS_CHANNEL = 1U;

    SSPSTAT = 0x00U;
    SSPCON1 = 0x00U;
    SSPCON2 = 0x00U;

    SSPSTATbits.SMP = 0U;
    SSPSTATbits.CKE = SPI_CFG_CLK_POLARITY;/// Clock select bit

    SSPCON1bits.SSPM = SPI_CFG_CLK_FREQ;
    SSPCON1bits.CKP = SPI_CFG_CLK_POLARITY;
    SSPCON1bits.SSPEN = 1U;
    SPI_internal_state = SPI_ready;

    PIR1bits.SSPIF = 0U;
    PIE1bits.SSPIE = 0U;
    IPR1bits.SSPIP = SPI_CFG_INTERRUPT_PRIORITY;
}

SPIState_t SPI_getStatus( void ){
    return SPI_internal_state;
}

SPIState_t SPI_CancelTransaction( void ){
	ASSERT( SPI_uninit == SPI_internal_state );
    if( SPI_ready != SPI_internal_state ){
        PIE1bits.SSPIE = 0U;
        PIR1bits.SSPIF = 0U;
        SPI_SS_CHANNEL = 1U;
        __rxBuffer = NULL;
        __txBuffer = NULL;
        __rxSize = 0U;
        __txSize = 0U;
        SPI_internal_state = SPI_cancel;
    }
    return SPI_internal_state;
}

SPIState_t SPI_TransactionAsync( SPI_Transaction_t* transaction ){
    if( SPI_ready == SPI_internal_state || SPI_cancel == SPI_internal_state ){
        SPI_internal_state = SPI_ready;
        if( (NULL != transaction->txBuffer) && 0U < transaction->txSize ){
            __txBuffer = transaction->txBuffer;
            __txSize = transaction->txSize;
        }else{
            __txBuffer = NULL;
            __txSize = 0U;
        }
        if( (NULL != transaction->rxBuffer) && (0U < transaction->rxSize) ){
            __rxBuffer = transaction->rxBuffer;
            __rxSize = transaction->rxSize;
        }else{
            __rxBuffer = NULL;
            __rxSize = 0U;
        }
        /// Something to transmit?
        if( 0 < (__txSize + __rxSize) ){
            SPI_InternalTransactionAsync();
        }
    }else{
        /* We can not proceed */
    }
    return SPI_internal_state;
}

SPIState_t SPI_TransactionSync( SPI_Transaction_t* transaction ){
    if( SPI_ready == SPI_internal_state || SPI_cancel == SPI_internal_state ){
        SPI_internal_state = SPI_ready;
     	if( (NULL != transaction->txBuffer) && 0U < transaction->txSize ){
            __txBuffer = transaction->txBuffer;
            __txSize = transaction->txSize;
        }else{
            __txBuffer = NULL;
            __txSize = 0U;
        }
        if( (NULL != transaction->rxBuffer) && (0U < transaction->rxSize) ){
            __rxBuffer = transaction->rxBuffer;
            __rxSize = transaction->rxSize;
        }else{
            __rxBuffer = NULL;
            __rxSize = 0U;
        }
        /// Something to transmit?
        if( 0 < (__txSize + __rxSize) ){
            SPI_InternalTransactionSync();
        }
    }else{
        /* We can not proceed */
    }
    return SPI_internal_state;
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/

static void SPI_InternalTransactionAsync( void ){
	ASSERT( SPI_ready == SPI_internal_state );
	ASSERT( 0 < (__txSize + __txSize) );

	SPI_internal_state = SPI_busy;
	PIR1bits.SSPIF = 0U;
    SPI_SS_CHANNEL = 0U;
	if( __txSize ){
		SSPBUF = *__txBuffer;
		__txBuffer++;
		__txSize--;
	}else{
		/// We have to transmit in order to receive
		SSPBUF = SPI_CFG_EMPTY_TX_BYTE;
	}
	PIE1bits.SSPIE = 1U;
}

static void SPI_InternalTransactionSync( void ){
    ASSERT( SPI_ready == SPI_internal_state );
	ASSERT( 0 < (__txSize + __rxSize) );

    SPI_internal_state = SPI_busy;
	PIR1bits.SSPIF = 0U;
    SPI_SS_CHANNEL = 0U;
	if( __txSize ){
		SSPBUF = *__txBuffer;
		__txBuffer++;
		__txSize--;
	}else{
		/// We have to transmit in order to receive
		SSPBUF = SPI_CFG_EMPTY_TX_BYTE;
	}
	while( 0 < (__txSize + __rxSize) ){
		if( PIR1bits.SSPIF ){
			PIR1bits.SSPIF = 0U;
            /// Save received byte
            if( (0U < __rxSize) ){
                *__rxBuffer = SSPBUF;
                __rxBuffer++;
                __rxSize--;
            }
            /// Set byte to transmit
            if( (0U < __txSize) ){
                SSPBUF = *__txBuffer;
                __txBuffer++;
                __txSize--;
            }
            /// RX pending but no Tx pending?
            else if( (0U < __rxSize) ){
                /// We have to transmit in order to receive
                SSPBUF = SPI_CFG_EMPTY_TX_BYTE;
            }
		}
	}
    SPI_SS_CHANNEL = 1U;
    __rxBuffer = NULL;
    __txBuffer = NULL;
    __rxSize = 0U;
    __txSize = 0U;
    SPI_internal_state = SPI_ready;
}

