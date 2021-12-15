#include "../../../include/bldrCtl/bldrCtl.h"
#include "../../../include/bldrCtl/bldrCtl_flashWrapper.h"

static bldr_state __state = bldr_init;

void BLDR_init(){
    BLDR_flashInit();
}

bldr_state BLDR_getState(){
    return __state;
}

bldr_state BLDR_MainHandler(){
    return __state;
}