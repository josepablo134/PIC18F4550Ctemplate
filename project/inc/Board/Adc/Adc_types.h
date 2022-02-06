#ifndef _ADC_TYPES_H_
#define _ADC_TYPES_H_
	#include "Adc_include.h"

	typedef enum AdcStatus{
			Adc_ready=0,
			Adc_busy,
			Adc_cancel,
			Adc_uninit,
	}AdcStatus_t;

	typedef uint8_t Adc_Channel;
	typedef uint16_t Adc_Counts_t;

#endif

