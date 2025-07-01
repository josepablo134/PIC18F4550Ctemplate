#ifndef COMM_H
#define COMM_H

	#include "ComM_cfg.h"
	#include "ComM_types.h"
	#include "ComM_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

    extern void ComM_Init(void);

    extern void ComM_Open(void);

    extern void ComM_Lock(void);

    extern void ComM_Unlock(void);

    extern ComM_LockStatus_t ComM_getLockStatus(void);

    /** Send a data package immediately and wait for confirmation ACK */
    extern ComM_Status_t ComM_Send( const ComM_Frame_t* payload , ComM_FrameConfirmation_t confirmation );

    /** Send an ACK package immediately */
    extern ComM_Status_t ComM_SendAck(void);

    /** Send a NACK package immediately */
    extern ComM_Status_t ComM_SendNack(void);

    /** Wait an ACK package, if received return OK, if timeout reached return NO_OK, timeout is in ms */
    extern ComM_Status_t ComM_WaitAck( ComM_timeout_t );

    /** Register a callback for the service id, whenver a new package is received the callback is invoked. */
    extern ComM_Status_t ComM_SetEventHandler( ComM_Event_id_t, ComM_EventHandler_t );

    /**
     * Infinite loop trying to receive a package, whenever a new package is received the corresponding service is invoked to process the package.
     * The service will respond an automatic NACK when the frame is rejected due to invalid content or structure.
     * If the client receives an ACK, this is due to the event handler, not the ComM.
     */
    extern void ComM_mainFunction( void );

#ifdef	__cplusplus
}
#endif

#endif
