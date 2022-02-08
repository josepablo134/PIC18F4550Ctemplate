#ifndef _SPI_H_
#define _SPI_H_

#ifdef _UART_H_
    #error "This driver is not compatible with UART driver"
#endif

	#include "SPI_cfg.h"
	#include "SPI_types.h"
	#include "SPI_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	extern void SPI_Init( void );

	extern void SPI_Open( void );

    extern SPIState_t SPI_Status( void );

    extern SPIState_t SPI_CancelTransaction( void );

	extern SPIState_t SPI_TransactionAsync( SPI_Transaction_t* transaction );

	extern SPIState_t SPI_TransactionSync( SPI_Transaction_t* transaction );

#ifdef	__cplusplus
}
#endif

#endif
