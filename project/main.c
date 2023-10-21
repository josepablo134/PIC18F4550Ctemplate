/*
 * File:   main.c
 * Author: josepablocb
 *
 * Created on December 14, 2021, 7:24 PM
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Dio.h"
#include "IIC.h"

#define ADXL345_SLAVE_ADDRESS           (0x53U << 1U)

#define ADXL345_REG_DEV_ID              (0x00U)
#define ADXL345_REG_POWER_CTL           (0x2DU)
#define ADXL345_REG_DATA_FORMAT         (0x31U)
#define ADXL345_REG_DATAX0              (0x32U)
#define ADXL345_REG_DATAX1              (0x33U)
#define ADXL345_REG_DATAY0              (0x34U)
#define ADXL345_REG_DATAY1              (0x35U)
#define ADXL345_REG_DATAZ0              (0x36U)
#define ADXL345_REG_DATAZ1              (0x37U)

#define ADXL345_REG_DATA_FORMAT_SET     (0x01U)
#define ADXL345_REG_POWER_CTL_SET       (0x08U)

#define DUMMY_BYTE                      (0xFFU);

#define BUFF_SIZE                       6U
IIC_byte_t buffer[BUFF_SIZE];

void main(void) {
    uint8_t  dev_id=0x00;
	IIC_Transaction_t iicTransaction = {
        .slave_address = (ADXL345_SLAVE_ADDRESS),
		.txBuffer = NULL,
		.txSize = 0U,
		.rxBuffer = NULL,
		.rxSize = 0U
	};

    Board_Init();

    Dio_Init();
    Dio_Open();

	IIC_Init();
	IIC_Open();

    Dio_WriteChannel( Dio_ch_0 , Dio_HIGH );

    /// Configure ADXL
    buffer[0] = ADXL345_REG_POWER_CTL;
    buffer[1] = ADXL345_REG_POWER_CTL_SET;/// +/- 4G range
    iicTransaction.txBuffer = buffer;
    iicTransaction.txSize = 2U;
    IIC_TransactionSync( &iicTransaction );

    buffer[0] = ADXL345_REG_DATA_FORMAT;
    buffer[1] = ADXL345_REG_DATA_FORMAT_SET;/// Medition mode
    iicTransaction.txBuffer = buffer;
    iicTransaction.txSize = 2U;
    IIC_TransactionSync( &iicTransaction );
    Dio_WriteChannel( Dio_ch_1 , Dio_HIGH );
    while(1){
        dev_id = 0x00;
                /// READ ID from ADXL345 (expected 0xE5U)
                buffer[0] = ADXL345_REG_DEV_ID;
                    iicTransaction.txBuffer = buffer;
                    iicTransaction.txSize = 1U;
                    iicTransaction.rxBuffer = buffer;
                    iicTransaction.rxSize = 1U;
                Dio_WriteChannel( Dio_ch_1 , Dio_LOW );//Signal Cancel Transaction Test
                IIC_TransactionAsync( &iicTransaction );
                __delay_us( 100 );
                IIC_CancelTransaction();
                Dio_WriteChannel( Dio_ch_1 , Dio_HIGH );
        /// READ ID from ADXL345 (expected 0xE5U)
        buffer[0] = ADXL345_REG_DEV_ID;
            iicTransaction.txBuffer = buffer;
            iicTransaction.txSize = 1U;
            iicTransaction.rxBuffer = buffer;
            iicTransaction.rxSize = 1U;
        IIC_TransactionAsync( &iicTransaction );
        while( IIC_ready != IIC_getStatus() ){ }

        dev_id = buffer[0];
        if( 0xE5U == dev_id ){
            LATAbits.LA4 = Dio_HIGH;
        }else{
            LATAbits.LA4 = Dio_LOW;
        }

        /// Read axis (from X0 buffer through Z1)
        buffer[0] = ADXL345_REG_DATAX0;
            iicTransaction.txBuffer = buffer;
            iicTransaction.txSize = 1U;
            iicTransaction.rxBuffer = buffer;
            iicTransaction.rxSize = BUFF_SIZE;
        IIC_TransactionAsync( &iicTransaction );
        while( IIC_ready != IIC_getStatus() ){ }
        __delay_ms( 500 );/// Debounce time
    }
}
