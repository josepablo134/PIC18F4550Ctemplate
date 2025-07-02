#ifndef APP_H
#define APP_H

    #include "Flash.h"
    #include "xModem.h"

    #define APP_CFG_BLDR_SERVICE_ID             (0xAB)

    #define APP_CFG_BLDR_CMD_ERASE              (0xA0)
    #define APP_CFG_BLDR_CMD_WRITE              (0xA1)
    #define APP_CFG_BLDR_CMD_READ               (0xA2)
    #define APP_CFG_BLDR_CMD_RESET              (0xA3)

    #define APP_CFG_PAGE_BUFFER_SIZE            (FLASH_CFG_PAGE_SIZE)

    typedef enum App_state_t{
        APP_STATE_INIT,
        APP_STATE_APP_CHECK,
        APP_STATE_APP_REPROGRAMMING,
        APP_STATE_APP_SAFE_STATE,
    }App_state_t;

    extern void App_Init( void );
    /**
     * Controls the flow between APP states
     */
    extern App_state_t App_mainFunction( App_state_t );

    /**
     * Wait upto 50ms for an ACK signal to be received, if not received
     * proceed to appcheck state. If received enable reprogramming session.
     */
    extern App_state_t App_State_Init( App_state_t );

    /**
     * Check a valid APP is installed and if so, pass control to APP.
     */
    extern App_state_t App_State_AppCheck( App_state_t );

    /**
     * Reprogramming session:
     *  - Stay here until the client requests finishing the reprogramming session. 
     */
    extern App_state_t App_State_AppReprogramming( App_state_t );

    /**
     * Safe state for an invalid App or where a not valid condition happens in the Application logic. 
     * This state disables the microcontroller putting it in sleep state (with interrupts disabled),
     * to get out of this state a POR or reset pin signal is required.
     */
    extern App_state_t App_State_AppSafeState( App_state_t );

#endif
