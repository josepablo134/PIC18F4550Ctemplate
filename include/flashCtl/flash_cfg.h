#ifndef _FLS_CTL_CFG_H_
#define	_FLS_CTL_CFG_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/* Minimum amount of bytes the MCU is able to write */
#define FLASH_CFG_PAGE_SIZE     						(32U)

/* Minimum amount of bytes the MCU is able to erase */
#define FLASH_CFG_SECTOR_SIZE   						(64U)

/* Minimum amount of bytes the Driver is able to write */
#define FLASH_CFG_VIRTUAL_PAGE_SIZE     				(FLASH_CFG_PAGE_SIZE * 1U)

/*
	Left shifting as divisor for virtual page size
	= log2( FLASH_CFG_VIRTUAL_PAGE_SIZE )
*/
#define FLASH_CFG_VIRTUAL_PAGE_DIV_LSFT_BITS			(5U)

/*
	Bit mask for virtual page values
*/
#define FLASH_CFG_VIRTUAL_PAGE_INVALID_BIT_MASK			((FlashSize_t) (FLASH_CFG_PAGE_SIZE-1U))

/* Minimum amount of bytes the Driver is able to erase */
#define FLASH_CFG_VIRTUAL_SECTOR_SIZE   				(FLASH_CFG_SECTOR_SIZE * 1U)

/*
	Left shifting as divisor for virtual sector size
	= log2( FLASH_CFG_VIRTUAL_SECTOR_SIZE )
*/
#define FLASH_CFG_VIRTUAL_SECTOR_DIV_LSFT_BITS			(6U)

/*
	Bit mask for virtual sector values
*/
#define FLASH_CFG_VIRTUAL_SECTOR_INVALID_BIT_MASK		((FlashSize_t) (FLASH_CFG_SECTOR_SIZE-1U))

#define FLASH_CFG_TOTAL_SIZE                			(32768U)

#define FLASH_CFG_VIRTUAL_SIZE              			(1024U)

#define FLASH_CFG_VIRTUAL_BASE_ADDR         			(0x7C00U)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
