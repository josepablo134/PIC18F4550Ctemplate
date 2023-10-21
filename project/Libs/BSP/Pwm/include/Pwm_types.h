#ifndef _PWM_TYPES_H_
#define _PWM_TYPES_H_
	#include "Pwm_include.h"

	typedef enum Pwm_Channel_t{
        Pwm_Ch_1=0,
        Pwm_Ch_2
    }Pwm_Channel_t;
    
	typedef uint16_t Pwm_DutyCycle_t; // 10bits PWM

#endif

