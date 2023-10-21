#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_

    #include "Adc_types.h"

    #define AdcCounts_INVALID                   ((AdcCounts_t)0xFFFFU)

    #define Adc_Ch_0                            ((AdcChannel_t)0U)
    #define Adc_Ch_1                            ((AdcChannel_t)1U)
    #define Adc_Ch_Max                          ((AdcChannel_t)2U)

    #define ADC_CFG_OPT_VREFminus_VREFminus     (1U)
    #define ADC_CFG_OPT_VREFminus_VDD           (0U)

    #define ADC_CFG_OPT_VREFplus_VREFplus       (1U)
    #define ADC_CFG_OPT_VREFplus_VSS            (0U)

    #define ADC_CFG_VREFminus                   ADC_CFG_OPT_VREFminus_VDD
    #define ADC_CFG_VREFplus                    ADC_CFG_OPT_VREFplus_VSS

    #define ADC_CFG_INTERRUPT_PRIORITY_OPT_LOW  (0U)
    #define ADC_CFG_INTERRUPT_PRIORITY_OPT_HIGH (1U)

    #define ADC_CFG_INTERRUPT_PRIORITY      ADC_CFG_INTERRUPT_PRIORITY_OPT_LOW

    #define ADC_CFG_CHANNELS_INIT   (0x0FU & ((uint8_t)~(Adc_Ch_Max)))

    #define ADC_CFG_COUNTS_INIT {\
        (AdcCounts_t)AdcCounts_INVALID,\
        (AdcCounts_t)AdcCounts_INVALID\
        }

    #define ADC_CFG_REG_ADCON2_OPT_ACQT_0TAD    (0U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_2TAD    (1U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_4TAD    (2U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_6TAD    (3U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_8TAD    (4U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_12TAD   (5U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_16TAD   (6U << 3U)
    #define ADC_CFG_REG_ADCON2_OPT_ACQT_20TAD   (7U << 3U)

    /**
     * Internal RC clock is: 31 kHz
     */
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_2  (0U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_8  (1U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_32 (2U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FRC0    (3U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_4  (4U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_16 (5U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FOSC_64 (6U)
    #define ADC_CFG_REG_ADCON2_OPT_ADCS_FRC1    (7U)

    #define ADC_CFG_REG_ADCON2_OPT_ADFM_RJ      (1U << 7U)
    #define ADC_CFG_REG_ADCON2_OPT_ADFM_LJ      (0U << 7U)

    /**
     * Try to get about 25 uS of acquisition time
     * (Datasheet, parameter 130 in Table 28-29)
     */
    #define ADC_CFG_REG_ADCON2_INIT     (\
            ADC_CFG_REG_ADCON2_OPT_ADFM_RJ |\
            ADC_CFG_REG_ADCON2_OPT_ADCS_FRC0 |\
            ADC_CFG_REG_ADCON2_OPT_ACQT_2TAD \
            )
#endif

