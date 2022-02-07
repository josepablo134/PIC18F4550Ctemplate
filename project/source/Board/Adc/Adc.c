#include "../../../inc/Board/Adc/Adc.h"
#include "../../../inc/Board/Adc/Adc_types.h"
#include "../../../inc/Board/Adc/Adc_include.h"

static AdcStatus_t AdcInternalState;

void Adc_Init(){
}

void Adc_Open(){
}

AdcCounts_t Adc_getCounts( AdcChannel_t channel ){
	return (AdcCounts_t)0x00U;
}

AdcStatus_t Adc_Status( AdcChannel_t channel ){
	return AdcInternalState;
}

AdcStatus_t Adc_ReadSync( AdcChannel_t channel ){
	return AdcInternalState;
}

AdcStatus_t Adc_ReadAsync( AdcChannel_t channel ){
	return AdcInternalState;
}

