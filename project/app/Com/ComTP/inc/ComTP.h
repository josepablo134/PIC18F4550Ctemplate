#ifndef _COMTP_H_
#define _COMTP_H_

	#include "ComTP_cfg.h"
	#include "ComTP_types.h"
	#include "ComTP_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

    extern void ComTP_Init(void);

    extern ComTP_Transaction_State_t ComTP_receiveState( void );

    extern ComTP_Status_t ComTP_receiveAsync( void* buffer, ComTP_buffer_size_t size, ComTP_EventHandler_t callback );

    extern void ComTP_CancelReceive( void );

    /**
     * Begin a TP transaction blicking the execution until the TP is completed or aborted.
     * For every frame sent, the TP expects an ACK to confirm the transaction, if this
     * ACK is not received then the transaction is cancelled.
     */
    extern ComTP_Status_t ComTP_transmitSync( void* buffer, ComTP_buffer_size_t size );

    /**
     * Service callback to be setup on the ComM:
     * This service handles requests to setup a transaction.
     */
    extern void ComTP_ComM_Service_SetUp( ComM_Frame_t* );

    /**
     * Service callback to be setup on the ComM:
     * This service handles requests to manage new data.
     */
    extern void ComTP_ComM_Service_Data( ComM_Frame_t* );

#ifdef	__cplusplus
}
#endif
    
#endif
