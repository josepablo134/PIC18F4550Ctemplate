#ifndef _DIO_TYPES_H_
#define _DIO_TYPES_H_
	#include "Dio_include.h"

	#define Dio_LOW					(0U)
	#define Dio_HIGH				(1U)
	#define Dio_INVALID				(2U)

	typedef uint8_t DioLevel_t;

	typedef enum DioChannel_t{
		Dio_ch_0,
		Dio_ch_1,
		Dio_ch_2,
		Dio_ch_3,
		Dio_ch_4,
		Dio_ch_5,
		Dio_ch_6,
		Dio_ch_7
	}DioChannel_t;

#endif

