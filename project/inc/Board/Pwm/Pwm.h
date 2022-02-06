#ifndef _PWM_H_
#define _PWM_H_

	#include "Pwm_cfg.h"
	#include "Pwm_types.h"
	#include "Pwm_include.h"

#ifdef	__cplusplus
extern "C" {
#endif
	extern void Pwm_Init( void );
	extern void Pwm_Open( void );
    extern void Pwm_Enable( void );
    extern void Pwm_Disable( void );
	extern void Pwm_SetDutyCycle( Pwm_Channel_t channel,
            Pwm_DutyCycle_t dutycycle );
#ifdef	__cplusplus
}
#endif

#endif
