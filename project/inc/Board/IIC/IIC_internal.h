#ifndef _IIC_INT_H_
#define _IIC_INT_H_

    #include "IIC_cfg.h"
    #include "IIC_types.h"

	extern volatile IICState_t IIC_internal_state;
	extern volatile IIC_byte_t*        __i2c_txBuffer;
	extern volatile IIC_byte_t*        __i2c_rxBuffer;
	extern volatile IIC_buffer_size_t  __i2c_txSize;
	extern volatile IIC_buffer_size_t  __i2c_rxSize;

	#define IIC_ISR(){\
		if( PIR1bits.SSPIF && ( IIC_busy == IIC_internal_state ) ){\
			PIR1bits.SSPIF = 0U;\
		}\
	}

#endif

