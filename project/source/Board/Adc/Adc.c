#ifndef _ADC_H_
#define _ADC_H_

	#include "../../../inc/Board/Adc/Adc.h"
	#include "../../../inc/Board/Adc/Adc_types.h"
	#include "../../../inc/Board/Adc/Adc_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	void Adc_Init();

	void Adc_Open();

	Adc_Counts_t Adc_getCounts( Adc_Channel channel );

	AdcStatus_t Adc_Status( Adc_Channel channel );

	AdcStatus_t Adc_ReadSync( Adc_Channel channel );

	AdcStatus_t Adc_ReadAsync( Adc_Channel channel );

#ifdef	__cplusplus
}
#endif

#endif
