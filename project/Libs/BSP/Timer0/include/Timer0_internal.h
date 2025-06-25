#ifndef _TMR0_INT_H_
#define _TMR0_INT_H_

#include "Timer0_cfg.h"

#define TMR0_ISR()              {\
    if( INTCONbits.TMR0IF ){\
        TMR0 += ((uint16_t)TMR0_SYSTICK_PRECHARGE);\
        INTCONbits.TMR0IF = 0;\
    }\
}

#endif