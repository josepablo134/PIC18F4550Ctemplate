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

	extern AdcCounts_t Adc_getCounts(void);

	extern AdcStatus_t Adc_Status(void);

	extern AdcStatus_t Adc_ConvertSync( AdcChannel_t channel );

	extern AdcStatus_t Adc_ConvertAsync( AdcChannel_t channel );

    extern AdcStatus_t Adc_CancelRequest(void);

#ifdef	__cplusplus
}
#endif

#endif
