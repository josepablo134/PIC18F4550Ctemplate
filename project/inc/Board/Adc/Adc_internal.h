#ifndef _ADC_INT_H_
#define _ADC_INT_H_

    #include "Adc_cfg.h"
    #include "Adc_types.h"

    extern volatile AdcStatus_t AdcInternalState;

    #define ADC_ISR()           {\
                if( PIR1bits.ADIF ){\
                    AdcInternalState = Adc_ready;\
                    PIE1bits.ADIE = 0U;\
                    PIR1bits.ADIF = 0U;\
                }\
            }

#endif
