#include "../../../inc/Board/IIC/IIC.h"

#define I2C_SLAVE_ADDR_MASK     (0xFEU)
#define I2C_WRITE_TO_SLAVE      (0U)
#define I2C_READ_FROM_SLAVE     (1U)

volatile IICState_t IIC_internal_state = IIC_uninit;
volatile IIC_byte_t*        __i2c_txBuffer;
volatile IIC_byte_t*        __i2c_rxBuffer;
volatile IIC_buffer_size_t  __i2c_txSize;
volatile IIC_buffer_size_t  __i2c_rxSize;
volatile IIC_byte_t         __i2c_address;

/*****************************************************************************
 *  Internal interfaces declaration
 *****************************************************************************/

static void IIC_InternalTransactionSync( void );
static void IIC_InternalTransactionAsync( void );

/*****************************************************************************
 *  Public interfaces definition
 *****************************************************************************/

void IIC_Init(void){
    __i2c_txBuffer = NULL;
    __i2c_rxBuffer = NULL;
    __i2c_txSize = 0U;
    __i2c_rxSize = 0U;
    __i2c_address = 0U;
}

void IIC_Open(void){
    ASSERT( IIC_uninit == IIC_internal_state );

    PORTBbits.RB0 = 0U;/// SDA (Serial Data)
    PORTBbits.RB1 = 0U;/// SCL (Serial Clock)

    LATBbits.LB0 = 0U;/// SDA (Serial Data)
    LATBbits.LB1 = 0U;/// SCK (Serial Clock)

    TRISBbits.RB0 = 1U;/// SDA (Serial Data)
    TRISBbits.RB1 = 1U;/// SCK (Serial Clock)

    SSPSTAT = 0x00U;
    SSPCON1 = 0x00U;
    SSPCON2 = 0x00U;

    SSPSTATbits.SMP = IIC_CFG_SLEW_RATE;
    SSPSTATbits.CKE = IIC_CFG_SMBus_MODE;

    SSPADD = (0xFFU) & IIC_CFG_CLOCK;

    SSPCON1bits.SSPM = 0x08U;/// From datasheet page 206: Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSPCON1bits.SSPEN = 1U;

    IIC_internal_state = IIC_ready;

    PIR1bits.SSPIF = 0U;
    PIE1bits.SSPIE = 0U;
    IPR1bits.SSPIP = IIC_CFG_INTERRUPT_PRIORITY;
}

IICState_t IIC_getStatus( void ){
    return IIC_internal_state;
}

IICState_t IIC_CancelTransaction( void ){
	ASSERT( IIC_uninit == IIC_internal_state );
    if( IIC_ready != IIC_internal_state ){
        PIE1bits.SSPIE = 0U;
        PIR1bits.SSPIF = 0U;
        __i2c_rxBuffer = NULL;
        __i2c_txBuffer = NULL;
        __i2c_rxSize = 0U;
        __i2c_txSize = 0U;
        IIC_internal_state = IIC_cancel;
    }
    return IIC_internal_state;
}

IICState_t IIC_TransactionAsync( IIC_Transaction_t* transaction ){
    if( IIC_ready == IIC_internal_state ||
            IIC_cancel == IIC_internal_state ||
            IIC_error == IIC_internal_state ){
        IIC_internal_state = IIC_ready;
        if( 0U < transaction->slave_address ){
            __i2c_address = (IIC_byte_t) I2C_SLAVE_ADDR_MASK & transaction->slave_address;
        }else{
            __i2c_address = 0x00U;
        }
        if( (NULL != transaction->txBuffer) && 0U < transaction->txSize ){
            __i2c_txBuffer = transaction->txBuffer;
            __i2c_txSize = transaction->txSize;
        }else{
            __i2c_txBuffer = NULL;
            __i2c_txSize = 0U;
        }
        if( (NULL != transaction->rxBuffer) && (0U < transaction->rxSize) ){
            __i2c_rxBuffer = transaction->rxBuffer;
            __i2c_rxSize = transaction->rxSize;
        }else{
            __i2c_rxBuffer = NULL;
            __i2c_rxSize = 0U;
        }
        /// Something to transmit?
        if( 0 < (__i2c_txSize + __i2c_rxSize) && 0U < __i2c_address ){
            IIC_InternalTransactionAsync();
        }
    }else{
        /* We can not proceed */
    }
    return IIC_internal_state;
}

IICState_t IIC_TransactionSync( IIC_Transaction_t* transaction ){
    if( IIC_ready == IIC_internal_state ||
            IIC_cancel == IIC_internal_state ||
            IIC_error == IIC_internal_state ){
        IIC_internal_state = IIC_ready;
        if( 0U < transaction->slave_address ){
            __i2c_address = I2C_SLAVE_ADDR_MASK & transaction->slave_address;
        }else{
            __i2c_address = 0x00U;
        }
     	if( (NULL != transaction->txBuffer) && 0U < transaction->txSize ){
            __i2c_txBuffer = transaction->txBuffer;
            __i2c_txSize = transaction->txSize;
        }else{
            __i2c_txBuffer = NULL;
            __i2c_txSize = 0U;
        }
        if( (NULL != transaction->rxBuffer) && (0U < transaction->rxSize) ){
            __i2c_rxBuffer = transaction->rxBuffer;
            __i2c_rxSize = transaction->rxSize;
        }else{
            __i2c_rxBuffer = NULL;
            __i2c_rxSize = 0U;
        }
        /// Something to transmit?
        if( 0 < (__i2c_txSize + __i2c_rxSize) ){
            IIC_InternalTransactionSync();
        }
    }else{
        /* We can not proceed */
    }
    return IIC_internal_state;
}

/*****************************************************************************
 *  Internal interfaces definition
 *****************************************************************************/

static void IIC_InternalTransactionAsync( void ){
	ASSERT( IIC_ready == IIC_internal_state );
	ASSERT( 0 < (__i2c_txSize + __i2c_txSize) );

	IIC_internal_state = IIC_busy;
	PIR1bits.SSPIF = 0U;
    /* Do something */
	PIE1bits.SSPIE = 1U;
}

static void IIC_InternalTransactionSync( void ){
    ASSERT( IIC_ready == IIC_internal_state );
	ASSERT( 0U < (__i2c_txSize + __i2c_rxSize) );
    ASSERT( 0U < (__i2c_address) );
    IIC_internal_state = IIC_busy;
	PIR1bits.SSPIF = 0U;

    /* Start Condition */
    SSPCON2bits.RCEN = 0U;/// Transmit mode
    SSPCON2bits.SEN = 1U;
        /// Wait for Start bit completion
        while( 0U == PIR1bits.SSPIF );
        PIR1bits.SSPIF = 0u;
        if( !SSPSTATbits.S ){
            /* Start bit not set */
            IIC_internal_state = IIC_error;
            return;
        }

    /* Send Control Byte */
    if( 0U < (__i2c_txSize) ){
        SSPBUF =  ( __i2c_address & I2C_SLAVE_ADDR_MASK ) | I2C_WRITE_TO_SLAVE;
        /* Send Bytes */
        while( 0U < __i2c_txSize ){
            if( 1U == PIR1bits.SSPIF ){
                PIR1bits.SSPIF = 0U;
                /* Read ACK to continue */
                if( SSPCON2bits.ACKSTAT ){
                    /* NACK received */
                    IIC_internal_state = IIC_error;
                    return;
                }
                SSPBUF = *__i2c_txBuffer;
                __i2c_txBuffer++;
                __i2c_txSize--;
            }
        }
        /// Wait for transaction to complete
        while( 0U == PIR1bits.SSPIF );
        PIR1bits.SSPIF = 0u;
            /* Read ACK to continue */
            if( SSPCON2bits.ACKSTAT ){
                /* NACK received */
                IIC_internal_state = IIC_error;
                return;
            }

        /// Something to receive?
        if( 0U < __i2c_rxSize ){
            SSPCON2bits.RSEN = 1U;/// Repeat Start Bit
            /// Wait for Repeated Start bit
            while( 0U == PIR1bits.SSPIF );
            PIR1bits.SSPIF = 0u;
            if( !SSPSTATbits.S ){
                /* Start bit not set */
                IIC_internal_state = IIC_error;
                return;
            }
        }
    }

    if( 0U < __i2c_rxSize ){
        /* Send Control Byte */
        SSPBUF =  ( __i2c_address & I2C_SLAVE_ADDR_MASK ) | I2C_READ_FROM_SLAVE;
        /// Wait for transaction to complete
        while( 0U == PIR1bits.SSPIF );
        PIR1bits.SSPIF = 0u;
        SSPCON2bits.RCEN = 1U;/// Receive mode
        
        while( 0U < __i2c_rxSize ){
            if( 1U == PIR1bits.SSPIF ){
                PIR1bits.SSPIF = 0U;
                    /// Write ACK to continue
                    SSPCON2bits.ACKDT = 1U;
                    SSPCON2bits.ACKEN = 1U;/// Send ACK to Slave
                    while( 0U == PIR1bits.SSPIF );
                    PIR1bits.SSPIF = 0u;
                *__i2c_rxBuffer = SSPBUF;
                __i2c_rxBuffer++;
                __i2c_rxSize--;
                if( 0U < __i2c_rxSize ){
                    SSPCON2bits.RCEN = 1U;/// Receive mode
                }
            }
        }
    }
    
    /* Send Stop Bit */
    SSPCON2bits.PEN = 1U;
        /// Wait for Stop bit completion
        while( 0U == PIR1bits.SSPIF );
        PIR1bits.SSPIF = 0u;
        if( !SSPSTATbits.P ){
            /* Start bit not set */
            IIC_internal_state = IIC_error;
            return;
        }
    IIC_internal_state = IIC_ready;
}

