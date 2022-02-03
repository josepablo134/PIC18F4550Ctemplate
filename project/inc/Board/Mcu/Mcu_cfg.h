#ifndef _MCU_CFG_H_
#define	_MCU_CFG_H_

    #define MCU_CFG_OPT_INT_OSC_8MHz            (0x07U)
    #define MCU_CFG_OPT_INT_OSC_4MHz            (0x07U)            
    #define MCU_CFG_OPT_INT_OSC_2MHz            (0x07U)
    #define MCU_CFG_OPT_INT_OSC_1MHz            (0x07U)
    #define MCU_CFG_OPT_INT_OSC_500kHz          (0x07U)
    #define MCU_CFG_OPT_INT_OSC_250kHz          (0x07U)
    #define MCU_CFG_OPT_INT_OSC_31kHz           (0x07U)

    #define MCU_CFG_INT_OSC                     MCU_CFG_OPT_INT_OSC_8MHz

    #define MCU_CFG_OPT_DEFAULT_OSC_PRIMARY     (0x00U)
    #define MCU_CFG_OPT_DEFAULT_OSC_TMR1        (0x01U)
    #define MCU_CFG_OPT_DEFAULT_OSC_INTERNAL    (0x02U)

    #define MCU_CFG_DEFAULT_OSC                 MCU_CFG_OPT_DEFAULT_OSC_PRIMARY

    #define MCU_CFG_OPT_LP_CLOCK_FROM_RC        (0x00U)
    #define MCU_CFG_OPT_LP_CLOCK_FROM_INTERNAL  (0x01U)

    #define MCU_CFG_LP_CLOCK                    MCU_CFG_OPT_LP_CLOCK_FROM_RC

#endif
