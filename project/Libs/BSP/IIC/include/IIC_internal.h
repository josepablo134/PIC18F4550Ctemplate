#ifndef _IIC_INT_H_
#define _IIC_INT_H_

    #include "IIC_cfg.h"
    #include "IIC_types.h"

    #define I2C_SLAVE_ADDR_MASK     (0xFEU)
    #define I2C_WRITE_TO_SLAVE      (0U)
    #define I2C_READ_FROM_SLAVE     (1U)

    extern volatile IICState_t         IIC_internal_state;
    extern volatile IICIntTransactionState_t IIC_int_trans_state;
    extern volatile IIC_byte_t*        __i2c_txBuffer;
    extern volatile IIC_byte_t*        __i2c_rxBuffer;
    extern volatile IIC_buffer_size_t  __i2c_txSize;
    extern volatile IIC_buffer_size_t  __i2c_rxSize;
    extern volatile IIC_byte_t         __i2c_address;


    inline void IIC_ISR_error_handler( void );
    inline void IIC_ISR( void );

#endif
