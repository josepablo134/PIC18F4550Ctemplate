#include "system.h"
#include "usb.h"

/*********************************************************************
* Function: void SYSTEM_Initialize( SYSTEM_STATE state )
*
* Overview: Initializes the system.
*
* PreCondition: None
*
* Input:  SYSTEM_STATE - the state to initialize the system into
*
* Output: None
*
********************************************************************/
void SYSTEM_Initialize( SYSTEM_STATE state )
{
    switch(state)
    {
        case SYSTEM_STATE_USB_START:
            SYSTEM_onStartEvent();
            break;
			
        case SYSTEM_STATE_USB_SUSPEND:
            SYSTEM_onSuspendEvent();
            break;
            
        case SYSTEM_STATE_USB_RESUME:
            SYSTEM_onResumeEvent();
            break;
    }
}


/**
 * @brief SYSTEM_onStartEvent
 *          Executed whenever USB device switch to SYSTEM_STATE_USB_START
 * 
*/
void SYSTEM_onStartEvent( void ){

}

/**
 * @brief SYSTEM_onSuspendEvent
 *          Executed whenever USB device switch to SYSTEM_STATE_USB_SUSPEND
 * 
*/
void SYSTEM_onSuspendEvent( void ){

}

/**
 * @brief SYSTEM_onResumeEvent
 *          Executed whenever USB device switch to SYSTEM_STATE_USB_RESUME
 * 
*/
void SYSTEM_onResumeEvent( void ){

}
