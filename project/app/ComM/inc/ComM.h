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

    extern ComM_Status_t ComM_Send( const ComM_Payload_t* payload );

    extern ComM_Status_t ComM_Receive( ComM_Payload_t* payload );

    extern ComM_Status_t ComM_SendAck(void);

    extern ComM_Status_t ComM_SendNack(void);

    extern ComM_Status_t ComM_WaitAck( ComM_timeout_t );

#ifdef	__cplusplus
}
#endif

#endif
