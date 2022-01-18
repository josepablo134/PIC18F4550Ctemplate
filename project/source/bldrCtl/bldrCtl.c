#include "../../../include/bldrCtl/bldrCtl.h"

static BldrState_t __state = BLDR_UNINIT;

void BLDR_init(){
}

BldrState_t BLDR_getState(){
    return __state;
}

BldrState_t BLDR_MainHandler(){
    return __state;
}