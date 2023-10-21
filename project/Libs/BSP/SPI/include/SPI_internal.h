#ifndef _SPI_INT_H_
#define _SPI_INT_H_

    #include "SPI_cfg.h"
    #include "SPI_types.h"

	extern volatile SPIState_t SPI_internal_state;
	extern volatile SPI_byte_t*        __txBuffer;
	extern volatile SPI_byte_t*        __rxBuffer;
	extern volatile SPI_buffer_size_t  __txSize;
	extern volatile SPI_buffer_size_t  __rxSize;

	#define SPI_ISR(){\
		if( PIR1bits.SSPIF && ( SPI_busy == SPI_internal_state ) ){\
			PIR1bits.SSPIF = 0U;\
            if( 0 < (__txSize + __rxSize) ){\
                /* Save received byte*/\
                if( (0U < __rxSize) ){\
                    *__rxBuffer = SSPBUF;\
                    __rxBuffer++;\
                    __rxSize--;\
                }\
                /* Set byte to transmit*/\
                if( (0U < __txSize) ){\
                    SSPBUF = *__txBuffer;\
                    __txBuffer++;\
                    __txSize--;\
                }\
                /* RX pending but no Tx pending?*/\
                else if( (0U < __rxSize) ){\
                    /* We have to transmit in order to receive*/\
                    SSPBUF = SPI_CFG_EMPTY_TX_BYTE;\
                } else {\
                    PIE1bits.SSPIE = 0U;\
                    SPI_SS_CHANNEL = 1U;\
                    __rxBuffer = NULL;\
                    __txBuffer = NULL;\
                    __rxSize = 0U;\
                    __txSize = 0U;\
                    SPI_internal_state = SPI_ready;\
                }\
            } else {\
                PIE1bits.SSPIE = 0U;\
                SPI_SS_CHANNEL = 1U;\
                __rxBuffer = NULL;\
                __txBuffer = NULL;\
                __rxSize = 0U;\
                __txSize = 0U;\
                SPI_internal_state = SPI_ready;\
            }\
        }\
	}

#endif

