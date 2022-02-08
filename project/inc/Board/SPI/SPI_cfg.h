#ifndef _SPI_CFG_H_
#define _SPI_CFG_H_

	#include "SPI_types.h"

    #define SPI_CFG_INTERRUPT_PRIORITY_OPT_LOW  (0U)
    #define SPI_CFG_INTERRUPT_PRIORITY_OPT_HIGH (1U)

    #define SPI_CFG_INTERRUPT_PRIORITY      SPI_CFG_INTERRUPT_PRIORITY_OPT_LOW

	/**
	 * LEADING = Clock in idle to rising edge
	 * TRAILING = Clock in idle to falling edge
	 */
	#define SPI_CFG_CLK_POL_LEADING     (0U)
	#define SPI_CFG_CLK_POL_TRAILING    (1U)

	#define SPI_CFG_CLK_POLARITY        ((uint8_t)SPI_CFG_CLK_POL_LEADING)

	/**
	 * Clock for SPI master mode from FOSC or TMR2
	 * if FOSC is 48Mhz
	 *  FOSC_64 -> 750Khz
	 *  FOSC_16 -> 3Mhz
	 *  FOSC_4  -> 12Mhz
	 * 
	 * if TMR2 {pre,post}scale is 1
	 *  then TMR2/2 -> FOSC/4/2 -> 12Mhz/2 -> 6Mhz
	 */
    #define SPI_CFG_CLK_OPT_TMR2_2      (3U)
    #define SPI_CFG_CLK_OPT_FOSC_64     (2U)
    #define SPI_CFG_CLK_OPT_FOSC_16     (1U)
    #define SPI_CFG_CLK_OPT_FOSC_4      (0U)

    #define SPI_CFG_CLK_FREQ            ((uint8_t)SPI_CFG_CLK_OPT_FOSC_64)

    #define SPI_CFG_EMPTY_TX_BYTE		(0x00U)

    #define SPI_SS_CHANNEL              LATAbits.LA5

#endif

