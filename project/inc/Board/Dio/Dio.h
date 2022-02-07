#ifndef _DIO_H_
#define _DIO_H_

	#include "Dio_cfg.h"
	#include "Dio_types.h"
	#include "Dio_include.h"

#ifdef	__cplusplus
extern "C" {
#endif

	void Dio_Init( void );

	void Dio_Open( void );

	DioLevel_t Dio_ReadChannel( DioChannel_t channel );

	void Dio_WriteChannel( DioChannel_t channel , DioLevel_t level );

#ifdef	__cplusplus
}
#endif

#endif
