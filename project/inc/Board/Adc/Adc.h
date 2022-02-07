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

	AdcCounts_t Adc_getCounts( AdcChannel_t channel );

	AdcStatus_t Adc_Status( AdcChannel_t channel );

	AdcStatus_t Adc_ReadSync( AdcChannel_t channel );

	AdcStatus_t Adc_ReadAsync( AdcChannel_t channel );

#ifdef	__cplusplus
}
#endif

#endif
