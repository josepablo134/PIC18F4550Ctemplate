# Baseline project for platform Microchip PIC18F4550 (MPLAB X project with XC8 compiler)

## Overview

This is a baseline project for the PIC18F4550 microcontroller with the following ready to use drivers:

 * __UART__ with dynamic configurable baudrate.
 * __ADC__ with static configurable number of channels.
 * __PWM__ (Pulse Width Modulation) with static configurable number of channels, using the highest possible frequency with the highest possible resolution (10 bits).
 * __DIO__ (Digital Input/Output) with static configurable number of channels.
 * Master mode __SPI__ (Serial Peripheral Interface) with static configurable clock frequency.
 * Master mode __IIC__ (Inter-Integrated Circuit) with static configurable clock frequency.
 * __Mcu__ (Micro-controller Unit) with static configurable clock signals to handle low-power modes (Sleep instruction for example).
 * __Timer 1__ with external 32.768 Khz crystal for real-time applications.
 * __Flash__ with static configurable memory map to read/erase/write data or code on the code flash memory.

Most of the base drivers come with __Sync__ and __Async__ interfaces to set requests to the peripheral and letting the ISRs (interrupt service routines) handle the responses. These ISRs (per driver) come with a static configurable priority.

This project also comes with an example of bootloader over UART ( :construction: work in progress) that shows the basic design and implementation of a bootloader software, control pass mechanism to application software, and modifying code flash routines.

## Schematic

This is the schematic of the platform based on the pinguino 18f4550 platform, which has a LED connected to the RA4 pin and the RE3 pin connected to a reset button.

![platform\_base](pic18f4550\_platform\_sch.png?raw=true "PICNEO image")

## Code Examples

### UART Hello world

```c
/*
 * File:   main.c
 */
#include "inc/Board/UART/UART.h"

char msg[] = "Hello world!!\n";

void main(void) {
    Board_Init();
    UART_Init();
    UART_Open( 115200U );

    while(1){
        UART_TransmitAsync( 
            (const uart_byte*) msg,
            sizeof( msg ) );

        /* Do something else */
        __delay_ms( 500 );
    }
}

/*
 * File: board_config.c
 */
#include "../../inc/Board/board_config.h"
#include "../../inc/Board/UART/UART_internal.h"

void __interrupt(high_priority)
Board_HP_ISR(void){
}

void __interrupt(low_priority)
Board_LP_ISR(void){
    UART_ISR()
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
}
```

### SPI Hello world

```c
/*
 * File:   main.c
 */
#include "inc/Board/SPI/SPI.h"

char spiBuffer[] = "Hello world!!\n";

void main(void) {
	SPI_Transaction_t spiTransaction = {
		.txBuffer = (SPI_byte_t*) spiBuffer,
		.rxBuffer = (SPI_byte_t*) spiBuffer,
		.txSize = sizeof(spiBuffer),
		.rxSize = sizeof(spiBuffer)
	};
    Board_Init();
    SPI_Init();
    SPI_Open();

    while(1){
        SPI_TransactionSync( &spiTransaction );

        /* Do something else */
        __delay_ms( 500 );
    }
}

/*
 * File: board_config.c
 */
#include "../../inc/Board/board_config.h"
#include "../../inc/Board/SPI/SPI_internal.h"

void __interrupt(high_priority)
Board_HP_ISR(void){
}

void __interrupt(low_priority)
Board_LP_ISR(void){
    SPI_ISR()
}

void Board_Init(){
    Mcu_Init();
    Mcu_Open();
}
```

### IIC (I2C) Hello world

```c
/*
 * File:   main.c
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/Board/Dio/Dio.h"
#include "inc/Board/IIC/IIC.h"

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
```

![ITST\_IIC\_I2C](itst/IIC\_itest_async\_cancel.png?raw=true "IIC Async Transaction cancel testing result")


### ADC + PWM

```c
/*
 * File:   main.c
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "Pwm.h"
#include "Adc.h"

void main(void) {
AdcCounts_t  counts;
    Board_Init();
    Adc_Init();
    Adc_Open();
    Pwm_Init();
    Pwm_Open();

    LATAbits.LA4 = 1U;

    while(1){
        Adc_ConvertSync( Adc_Ch_0 );
        counts = Adc_getCounts();

        Pwm_SetDutyCycle( Pwm_Ch_1,
            (Pwm_DutyCycle_t) counts );
        
        /* Do something else */
        __delay_ms( 500 );
        LATAbits.LA4 = !PORTAbits.RA4;
    }
}
```

![ITST\_ADc\_PWM](itst/ADC\_PWM\_itest.png?raw=true "ADC+PWM Test Result")
