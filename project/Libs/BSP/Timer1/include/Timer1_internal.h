#ifndef _TMR1_INT_H_
#define _TMR1_INT_H_

#include "Timer1_cfg.h"

#define TMR1_ISR()              {\
    if( PIR1bits.TMR1IF ){\
        TMR1 += ((uint16_t)TMR1_SYSTICK_PRECHARGE);\
        PIR1bits.TMR1IF = 0;\
    }\
}

#endif