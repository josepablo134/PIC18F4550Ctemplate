#include "project_macros.h"
#include "board_config.h"
#include "flash.h"

FlashStdReturn_t FlashErase( FlashAddress_t addr , FlashSize_t length ){
    uint16_t  count;
    ASSERT( 0 != length );

    /// Validate length and alignment
    if( (FLASH_CFG_TOTAL_SIZE < (addr + length) ) ||
		(FLASH_CFG_VIRTUAL_SECTOR_INVALID_BIT_MASK & addr ) ||
		(FLASH_CFG_VIRTUAL_SECTOR_INVALID_BIT_MASK & length )
    ){
        return FLS_NOT_OK;
    }

    addr = addr + FLASH_CFG_VIRTUAL_BASE_ADDR;
    
	/// Divide by SECTOR SIZE
    count = (FlashSize_t)(length >> \
		FLASH_CFG_VIRTUAL_SECTOR_DIV_LSFT_BITS
	);

    //EECON1bits.EEPGD = 1;
    //EECON1bits.CFGS = 0;
    //EECON1bits.WREN = 1;
    //EECON1bits.FREE = 1;
    EECON1 = 0x94;
    while( count-- ){
        TBLPTR = addr;  //Disable interrupts
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1; //CPU stall until Flash finishes operation
        if( EECON1bits.WRERR ){
            return FLS_NOT_OK;
        }
        //Enable interrupts again
        addr += FLASH_CFG_SECTOR_SIZE;
    }    
    return FLS_OK;
}

FlashStdReturn_t FlashWrite( FlashAddress_t addr,
        FlashSize_t length ,
        const uint8_t* data ){
    uint8_t const *pdata = data;
    uint16_t count;
    
    ASSERT( NULL != data );
    ASSERT( 0 != length );

    /// Validate length alignment
    if( (FLASH_CFG_TOTAL_SIZE < (addr + length) ) ||
		(FLASH_CFG_VIRTUAL_PAGE_INVALID_BIT_MASK & addr ) ||
		(FLASH_CFG_VIRTUAL_PAGE_INVALID_BIT_MASK & length )
    ){
        return FLS_NOT_OK;
    }

    addr = addr + FLASH_CFG_VIRTUAL_BASE_ADDR;
    count = (FlashSize_t)(length >> \
		FLASH_CFG_VIRTUAL_PAGE_DIV_LSFT_BITS
	);
    
    EECON1 = 0x84;
    //EECON1bits.EEPGD = 1;
    //EECON1bits.CFGS = 0;
    //EECON1bits.FREE = 0;
    //EECON1bits.WREN = 1;
    
    while( count-- ){
        TBLPTR = addr;
        for( FlashSize_t i=0; i< FLASH_CFG_PAGE_SIZE; i++ ){
            TABLAT = *pdata;
            pdata++;
            asm("TBLWT*+");
        }
        TBLPTR = addr;
        //Disable interrupts
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1; //CPU stall until Flash finishes operation
        if( EECON1bits.WRERR ){
            return FLS_NOT_OK;
        }
        //Enable interrupts again
        addr += FLASH_CFG_PAGE_SIZE;
    }
    
    return FLS_OK;
}

FlashStdReturn_t FlashRead( FlashAddress_t addr, FlashSize_t length , uint8_t* data ){
    uint8_t *pdata = data;
    
    ASSERT( NULL != data );
    ASSERT( 0 != length );

    /// Validate length alignment
    if( FLASH_CFG_TOTAL_SIZE < (addr + length) ){
        return FLS_NOT_OK;
    }

    addr = addr + FLASH_CFG_VIRTUAL_BASE_ADDR;
    
    EECON1 = 0x80;
    //EECON1bits.EEPGD = 1;
    //EECON1bits.CFGS = 0;
    //EECON1bits.FREE = 0;
    //EECON1bits.WREN = 0;

    TBLPTR = addr;
    while( length-- ){
        asm("TBLRD*+");
        *pdata = TABLAT;
		pdata++;
    }
    return FLS_OK;
}

FlashStdReturn_t FlashValidate( FlashAddress_t addr , FlashSize_t length , const uint8_t* data ){
	FlashStdReturn_t retVal = FLS_NOT_OK;
    uint8_t *pdata = (uint8_t*) data;
    
    ASSERT( NULL != data );
    ASSERT( 0 != length );

    /// Validate length alignment
    if( FLASH_CFG_TOTAL_SIZE < (addr + length) ){
        return FLS_NOT_OK;
    }

    addr = addr + FLASH_CFG_VIRTUAL_BASE_ADDR;
    
    EECON1 = 0x80;
    //EECON1bits.EEPGD = 1;
    //EECON1bits.CFGS = 0;
    //EECON1bits.FREE = 0;
    //EECON1bits.WREN = 0;

    TBLPTR = addr;

	do{
        asm("TBLRD*+");
    }while( ( --length ) && ( TABLAT == *(pdata++) ) );

	if( 0U < length ){
		retVal = FLS_NOT_OK;
	}else{
    	retVal = FLS_OK;
	}
	return retVal;
}
