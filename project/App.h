#ifndef APP_H
#define APP_H

    #include "Flash.h"
    #include "xsModem.h"

    #define APP_ERR_CHAR                        (0x18U)
    #define APP_CFG_PAGE_BUFFER_SIZE            (128U)

    extern void App_Init( void );

    /**
     * Check a valid APP is installed and if so, pass control to APP.
     */
    extern void App_State_AppCheck( void );

    /**
     * Reprogramming session:
     *  - Stay here until the client requests finishing the reprogramming session. 
     */
    extern void App_State_AppReprogramming( void );

#endif
