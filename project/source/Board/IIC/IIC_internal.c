#include "../../../inc/Board/IIC/IIC.h"
#include "../../../inc/Board/IIC/IIC_internal.h"

inline void IIC_ISR_error_handler( void ){
    IIC_internal_state = IIC_error;
    PIE1bits.SSPIE = 0U;//finish transaction
    SSPCON1bits.SSPEN = 0U; // Cancel all transactions
}

inline void IIC_ISR( void ){
    if( PIR1bits.SSPIF && ( IIC_busy == IIC_internal_state ) ){
        PIR1bits.SSPIF = 0U;
        switch ( IIC_int_trans_state )
        {
            case IIC_start_bit:{
                if( !SSPSTATbits.S ){
                    return IIC_ISR_error_handler();
                }
                if( 0U < __i2c_txSize ){
                    SSPBUF =  ( __i2c_address & I2C_SLAVE_ADDR_MASK ) | I2C_WRITE_TO_SLAVE;
                    IIC_int_trans_state = IIC_transmition_ongoing;
                }else if( 0U < __i2c_rxSize ){
                    SSPBUF =  ( __i2c_address & I2C_SLAVE_ADDR_MASK ) | I2C_READ_FROM_SLAVE;
                    IIC_int_trans_state = IIC_reception_preamble;
                }else{
                    return IIC_ISR_error_handler();
                }
            }
            break;
            case IIC_repeat_start_bit:{
                if( !SSPSTATbits.S ){
                    return IIC_ISR_error_handler();
                }
                if( 0U < __i2c_rxSize ){
                    SSPBUF =  ( __i2c_address & I2C_SLAVE_ADDR_MASK ) | I2C_READ_FROM_SLAVE;
                    IIC_int_trans_state = IIC_reception_preamble;
                }else{
                    return IIC_ISR_error_handler();
                }
            }
            break;
            case IIC_transmition_ongoing:{
                if( 0U < __i2c_txSize ){
                    /* Read ACK to continue */
                    if( SSPCON2bits.ACKSTAT ){
                        /* NACK received */
                        return IIC_ISR_error_handler();
                    }
                    SSPBUF = *__i2c_txBuffer;
                    __i2c_txBuffer++;
                    __i2c_txSize--;
                }else{
                    /* Read ACK to continue */
                    if( SSPCON2bits.ACKSTAT ){
                        /* NACK received */
                        return IIC_ISR_error_handler();
                    }

                    /// Something to receive?
                    if ( 0U < __i2c_rxSize ){
                        IIC_int_trans_state = IIC_repeat_start_bit;
                        SSPCON2bits.RSEN = 1U;/// Repeat Start Bit
                    } else {
                        /* Send Stop Bit */
                        SSPCON2bits.PEN = 1U;
                        IIC_int_trans_state = IIC_stop_bit;
                    }
                }
            }
            break;
            case IIC_reception_preamble:{
                if( SSPCON2bits.ACKSTAT ){
                    /* NACK received */
                    return IIC_ISR_error_handler();
                }else{
                    IIC_int_trans_state = IIC_reception_ongoing;
                    SSPCON2bits.RCEN = 1U;/// Receive mode
                }
            }
            case IIC_reception_ongoing:{
                if( 0U == SSPSTATbits.BF ){
                    // Wait for receive event
                    SSPCON2bits.RCEN = 1U;/// Receive mode
                    return;
                }
                if( 1U < __i2c_rxSize ){
                    SSPCON2bits.ACKDT = 0U;/// Write ACK to continue
                    SSPCON2bits.ACKEN = 1U;/// Send ACK to Slave
                    *__i2c_rxBuffer = SSPBUF;
                    __i2c_rxBuffer++;
                    __i2c_rxSize--;
                }else{
                    // On Last byte to receive
                    SSPCON2bits.ACKDT = 1U;
                    SSPCON2bits.ACKEN = 1U;/// Send NACK to finish transaction
                    IIC_int_trans_state = IIC_reception_epilog;
                        *__i2c_rxBuffer = SSPBUF;
                        __i2c_rxBuffer++;
                        __i2c_rxSize--;
                }
            }
            break;
            case IIC_reception_epilog:{
                SSPCON2bits.PEN = 1U;
                IIC_int_trans_state = IIC_stop_bit;
            }
            break;
            case IIC_stop_bit:{
                if( !SSPSTATbits.P ){
                    /* Stop bit not set */
                    return IIC_ISR_error_handler();
                }else{
                    /// Conclude transaction
                    PIE1bits.SSPIE = 0U;
                    IIC_internal_state = IIC_ready;
                }
            }
            break;
        }
    }
}
