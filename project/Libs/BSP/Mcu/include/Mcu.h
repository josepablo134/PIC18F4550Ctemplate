#ifndef _MCU_H_
#define	_MCU_H_

    #include "Mcu_include.h"
    #include "Mcu_cfg.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    extern void Mcu_Init(void);
    
    extern void Mcu_Open(void);

    extern void Mcu_Reset(void);
    
    extern void Mcu_Sleep(void);
    
    extern void Mcu_PowerOff(void);

#ifdef	__cplusplus
}
#endif
    
#endif
