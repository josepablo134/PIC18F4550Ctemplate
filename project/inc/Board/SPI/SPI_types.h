#ifndef _SPI_TYPES_H_
#define _SPI_TYPES_H_

	#include <stdint.h>
	#include "SPI_include.h"

	typedef uint8_t SPI_byte_t;
	typedef uint8_t SPI_buffer_size_t;

	typedef enum SPIState_t{
		SPI_ready,
		SPI_busy,
		SPI_cancel,
		SPI_uninit,
	}SPIState_t;

	typedef struct SPI_Transaction_t{
		SPI_byte_t* txBuffer;
		SPI_byte_t* rxBuffer;
		SPI_buffer_size_t txSize;
		SPI_buffer_size_t rxSize;
	}SPI_Transaction_t;

#endif

