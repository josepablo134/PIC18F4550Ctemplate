#ifndef _DIO_H_
#define _DIO_H_

	#include "Dio_cfg.h"
	#include "Dio_types.h"
	#include "Dio_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	extern void Dio_Init( void );

	extern void Dio_Open( void );

	extern DioLevel_t Dio_ReadChannel( DioChannel_t channel );

	extern void Dio_WriteChannel( DioChannel_t channel , DioLevel_t level );

#ifdef	__cplusplus
}
#endif

#endif
