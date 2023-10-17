#ifndef _IIC_H_
#define _IIC_H_

#ifdef _UART_H_
    #error "This driver is not compatible with the UART driver"
#endif

#ifdef _SPI_H_
    #error "This driver is not compatible with the SPI driver"
#endif

	#include "IIC_cfg.h"
	#include "IIC_types.h"
	#include "IIC_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	extern void IIC_Init( void );

	extern void IIC_Open( void );

    extern IICState_t IIC_getStatus( void );

    extern IICState_t IIC_CancelTransaction( void );

	extern IICState_t IIC_TransactionAsync( IIC_Transaction_t* transaction );

	extern IICState_t IIC_TransactionSync( IIC_Transaction_t* transaction );

#ifdef	__cplusplus
}
#endif

#endif
