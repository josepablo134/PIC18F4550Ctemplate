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

	AdcCounts_t Adc_getCounts(void);

	AdcStatus_t Adc_Status(void);

	AdcStatus_t Adc_ConvertSync( AdcChannel_t channel );

	AdcStatus_t Adc_ConvertAsync( AdcChannel_t channel );

    AdcStatus_t Adc_CancelRequest(void);

#ifdef	__cplusplus
}
#endif

#endif
