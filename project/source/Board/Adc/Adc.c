#include "../../../inc/Board/Adc/Adc.h"
#include "../../../inc/Board/Adc/Adc_types.h"
#include "../../../inc/Board/Adc/Adc_include.h"

volatile AdcStatus_t AdcInternalState = Adc_uninit;

void Adc_Init(){
    AdcInternalState = Adc_busy;
}

void Adc_Open(){
    ASSERT( Adc_uninit != AdcInternalState );

    PIR1bits.ADIF = 0U;
    PIE1bits.ADIE = 0U;
    IPR1bits.ADIP = ADC_CFG_INTERRUPT_PRIORITY;

    ADCON0 = 0x00;

    /* Configure channels and references (Vss and Vdd)*/
    ADCON1bits.VCFG0 = ADC_CFG_VREFplus;
    ADCON1bits.VCFG1 = ADC_CFG_VREFminus;
    ADCON1bits.PCFG = ADC_CFG_CHANNELS_INIT;

    ADCON2 = ADC_CFG_REG_ADCON2_INIT;

    ADCON0bits.ADON = 1U;/// Enable ADC peripheral

    AdcInternalState = Adc_ready;
}

AdcCounts_t Adc_getCounts(){
    ASSERT( Adc_uninit != AdcInternalState );
    AdcCounts_t retVal = ADRES;
    return retVal;
}

AdcStatus_t Adc_Status(){
    ASSERT( Adc_uninit != AdcInternalState );
	return AdcInternalState;
}

AdcStatus_t Adc_ConvertSync( AdcChannel_t channel ){
    ASSERT( Adc_uninit != AdcInternalState );
    ASSERT( channel < Adc_Ch_Max );
    if( (Adc_ready == AdcInternalState) && (!ADCON0bits.NOT_DONE) ){
        AdcInternalState = Adc_busy;

        ADCON0bits.CHS = channel;
        ADCON0bits.GO = 1U;

        while( ADCON0bits.NOT_DONE )
        {}

        AdcInternalState = Adc_ready;
    }else{
        /* Ignore request */
    }
	return AdcInternalState;
}

AdcStatus_t Adc_ConvertAsync( AdcChannel_t channel ){
    ASSERT( Adc_uninit != AdcInternalState );
    ASSERT( channel < Adc_Ch_Max );
    if( (Adc_ready == AdcInternalState) && (!ADCON0bits.NOT_DONE) ){
        AdcInternalState = Adc_busy;

        PIR1bits.ADIF = 0U;
        PIE1bits.ADIE = 1U;

        ADCON0bits.CHS = channel;
        ADCON0bits.GO = 1U;
        /// Let the ISR do its work
    }else{
        /* Ignore request */
    }
	return AdcInternalState;
}

AdcStatus_t Adc_CancelRequest(){
    PIE1bits.ADIE = 0U;
    PIR1bits.ADIF = 0U;
    AdcInternalState = Adc_cancel;
    return AdcInternalState;
}
