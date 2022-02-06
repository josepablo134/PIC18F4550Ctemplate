#ifndef _ADC_H_
#define _ADC_H_

	#include "Adc_cfg.h"
	#include "Adc_types.h"
	#include "Adc_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	extern void Adc_Init( void );

	extern void Adc_Open( void );

	extern Adc_Counts_t Adc_getCounts( Adc_Channel channel );

	extern AdcStatus_t Adc_Status( Adc_Channel channel );

	extern AdcStatus_t Adc_ReadSync( Adc_Channel channel );

	extern AdcStatus_t Adc_ReadAsync( Adc_Channel channel );

#ifdef	__cplusplus
}
#endif

#endif
