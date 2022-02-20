#ifndef _IIC_CFG_H_
#define _IIC_CFG_H_

	#include "IIC_types.h"

    #define IIC_CFG_INTERRUPT_PRIORITY_OPT_LOW  (0U)
    #define IIC_CFG_INTERRUPT_PRIORITY_OPT_HIGH (1U)

    #define IIC_CFG_INTERRUPT_PRIORITY      IIC_CFG_INTERRUPT_PRIORITY_OPT_LOW

    #define IIC_CFG_SLEW_RATE_OPT_STD_SPEED_MODE	(0U)
    #define IIC_CFG_SLEW_RATE_OPT_HIGH_SPEED_MODE	(1U)

    #define IIC_CFG_SLEW_RATE				IIC_CFG_SLEW_RATE_OPT_STD_SPEED_MODE

    #define IIC_CFG_SMBus_MODE_OPT_DISABLED		(0U)
    #define IIC_CFG_SMBus_MODE_OPT_ENABLED		(1U)

    #define IIC_CFG_SMBus_MODE				IIC_CFG_SMBus_MODE_OPT_DISABLED

	/**
	 * Clock for IIC master mode from FOSC/(4 * (IIC_CFG_CLOCK + 1) )
     * If FOSC = 48Mhz and IIC_CFG_CLOCK = 1
     * => SCL freq. is 6Mhz
     * but if IIC_CFG_CLOCK = 255
     * => SCL freq. is 46,875 Hz
	 */
    #define IIC_CFG_CLOCK                       (255U)

#endif

