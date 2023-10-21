#ifndef _KEYPAD_H_
#define	_KEYPAD_H_

#include "Keypad_include.h"
#include "Keypad_types.h"
#include "Keypad_cfg.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Keypad_Init
 * 
 * @brief Initialize only internal driver state.
 */
void Keypad_Init(void);

/**
 * Keypad_Open
 * 
 * @brief Configure all needed peripherals.
 */
void Keypad_Open(void);

/**
 * Keypad_captureKey
 * 
 * @brief Read the key from the port,
 * assuming no debounce time needed.
 * 
 * Be careful with real time applications,
 * this function introduces a mandatory delay
 * in order to work propertly, the delay
 * depends on the main CPU and the 
 * port STROBE reaction time, this minimum delay
 * is configured in the Keypad_config.h file.
 * 
 * @param void
 * 
 * @return Keypad_key_t either valid or invalid combination.
 */
Keypad_key_t Keypad_captureKey(void);

/**
 * Keypad_Key2Ascii
 * 
 * @brief Convert a keypad key into a generic ascii character.
 * If the key pressed is an invalid key, the function returns the
 * INVALID_ASCII_CHAR. Valid ascii keys are defined in
 * Keypad_cfg.h
 * 
 * @param key
 * 
 * @return INVALID_ASCII_CHAR in case of error or
 * any valid generic chararacter in case of success.
 */
char Keypad_Key2Ascii( Keypad_key_t key );

/**
 * Keypad_Nibble2BitIndex
 * 
 * @brief Convert any bit enabled in the nibble to the index of the bit.
 * 
 * @param nibble
 * @return KEYPAD_KEY_INDEX_INVALID on fail, index from
 * 0 to 3 in case of success.
 */
Keypad_key_index_t Keypad_Nibble2BitIndex( Keypad_nibble_t nibble );

#ifdef	__cplusplus
}
#endif

#endif
