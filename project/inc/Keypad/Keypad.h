#ifndef _KEYPAD_H_
#define	_KEYPAD_H_

#include "Keypad_include.h"
#include "Keypad_types.h"

#ifdef	__cplusplus
extern "C" {
#endif

void Keypad_init(void);
Keypad_key_t Keypad_captureKey(void);
char Keypad_key2ascii( Keypad_key_t key );

#ifdef	__cplusplus
}
#endif

#endif
