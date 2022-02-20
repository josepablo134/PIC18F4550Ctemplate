#ifndef _IIC_TYPES_H_
#define _IIC_TYPES_H_

	#include <stdint.h>
	#include "IIC_include.h"

	typedef uint8_t IIC_byte_t;
	typedef uint8_t IIC_buffer_size_t;

	typedef enum IICState_t{
		IIC_ready,
		IIC_busy,
		IIC_cancel,
        IIC_error,
		IIC_uninit,
	}IICState_t;
    
	typedef struct IIC_Transaction_t{
        IIC_byte_t  slave_address;
		IIC_byte_t* txBuffer;
		IIC_byte_t* rxBuffer;
		IIC_buffer_size_t txSize;
		IIC_buffer_size_t rxSize;
	}IIC_Transaction_t;

#endif

