#ifndef _KEYPAD_CFG_H_
#define	_KEYPAD_CFG_H_

    #include "Keypad_types.h"

    /**
     * Minimum delay time to let the signal reach minimum
     * high voltage for TTL logic (about 2 volts + extra secure delta)
     * If the CPU runs at the maximum frequency (48Mhz), the port strobe
     * is so quick that the port itself is not able to capture the signal.
     * In order to obtain better results, measure the strobe signal with a
     * probe and adjust the minimum delay time accordingly.
     * 
     * For 48Mhz 2 micro seconds is enough to reach about 2.3 volts,
     * but using 4 micro seconds guarantees no glitches in the signal.
     * */
    #define KEYPAD_CFG_MIN_READ_DELAY_US       (4U)

    #define KEYPAD_CFG_ROWS                     (4U)
    #define KEYPAD_CFG_COLUMNS                  (4U)
    #define KEYPAD_CFG_INVALID_CHAR             ('X')
    #define KEYPAD_CFG_KEYPAD_KEYMAP_CONST {\
            {'1','2','3','A'},\
            {'4','5','6','B'},\
            {'7','8','9','C'},\
            {'*','0','#','D'}\
            }

#endif
