#include "Keypad.h"

static const char keypad_keymap[ KEYPAD_CFG_ROWS ][ KEYPAD_CFG_COLUMNS ] = \
KEYPAD_CFG_KEYPAD_KEYMAP_CONST;

void Keypad_Init(){
    /* Nothing to do here */
}

void Keypad_Open(void){
    /// ADCON1 is handled by ADC driver
    PORTB = 0x00;
    TRISB = 0xF0;
    LATB = 0xF0;///Pull-up control
    // Internal weak pull-up enabled
    // Only affects input bits
    INTCON2bits.NOT_RBPU = 0;
}

Keypad_key_t Keypad_captureKey(){
    Keypad_key_t key = 0U;

#if 0 
    /*
     * Keypad_Open configures this
     * so is no longer necessary.
     */
    TRISB = 0xF0;
    LATB = 0xF0;///Pull-up control
    __delay_us( KEYPAD_CFG_MIN_READ_DELAY_US );
#endif
    key |= PORTB & 0xF0;

    TRISB = 0x0F;
    LATB = 0x0F;///Pull-up control
    __delay_us( KEYPAD_CFG_MIN_READ_DELAY_US );
    key |= PORTB & 0x0F;

    /* Restart original port configuration */
    TRISB = 0xF0;
    LATB = 0xF0;///Pull-up control
    /// Pressed = 1, unpress = 0
    key = ~key;
    return key;
}

Keypad_key_index_t Keypad_Nibble2BitIndex( Keypad_nibble_t nibble ){
    Keypad_key_index_t retVal;
    switch( nibble ){
        case 0x01:
            retVal = 0x00;
            break;
        case 0x02:
            retVal = 0x01;
            break;
        case 0x04:
            retVal = 0x02;
            break;
        case 0x08:
            retVal = 0x03;
            break;
        default:
            retVal = KEYPAD_KEY_INDEX_INVALID;
    }
    return retVal;
}

char Keypad_Key2Ascii( Keypad_key_t key ){
    char retVal = KEYPAD_CFG_INVALID_CHAR;
    
    Keypad_key_index_t rowI = Keypad_Nibble2BitIndex(
            (Keypad_nibble_t)(key & 0x0F) );
    Keypad_key_index_t colI = Keypad_Nibble2BitIndex(
                (Keypad_nibble_t)( (key>>4U) & 0x0F) );
    
    if( (KEYPAD_KEY_INDEX_INVALID != rowI) &&
            (KEYPAD_KEY_INDEX_INVALID != colI) ){
        retVal = keypad_keymap[rowI][colI];
    }else{
        retVal = KEYPAD_CFG_INVALID_CHAR;
    }
    
    return retVal;
}
