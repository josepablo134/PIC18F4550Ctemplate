#include "../../inc/Keypad/Keypad.h"

void Keypad_init(){
    /// Internal weak pull-up enabled
    // Only affects input bits
    PORTB = 0x00;
    LATB = 0x00;
    TRISB = 0x0F;
    INTCON2bits.NOT_RBPU = 0;
}

Keypad_key_t Keypad_captureKey(){
    Keypad_key_t key;

    TRISB = 0x0F;
    LATB = 0x0F;///Pull-up control
    key = PORTB & 0x0F;

    TRISB = 0xF0;
    LATB = 0xF0;///Pull-up control
    key |= PORTB & 0xF0;
    
    /// Pressed = 1, unpress = 0
    key = ~key;
    return key;
}

char Keypad_key2ascii( Keypad_key_t key ){
    const char key2ascii[ 16U ] = {
        '1','2','3','A',
        '4','5','6','B',
        '7','8','9','C',
        '*','0','#','D'
    };
    char kchar = ' ';
    if( key ){
        uint8_t index;
        uint8_t byte = key & 0x0F;

        index = ( ( byte > 8 )? 3 : byte>>1U ) + 1U;
        byte = key >> 4U;
        index *= ( ( byte > 8 )? 3 : byte>>1U ) + 1U;
        
        index -= 1;
        kchar = key2ascii[ index ];
    }
    return kchar;
}
