#ifndef APP_H
#define APP_H

    #include "ComM.h"
    #include "ComTP.h"
    #include "Flash.h"

    /** Wait time in ms. This is the time the BL waits for an ACK signal to start the reprogramming session. */
    #define APP_CFG_INIT_WAIT_TIME              (50U)
    /** COMTP_DATA -> 0b1000 */
    #define APP_CFG_COMTP_SERVICE_ID_DATA       (COMTP_CFG_SERVICE_ID_DATA)
    /** COMTP_SETUP -> 0b1100 */
    #define APP_CFG_COMTP_SERVICE_ID_SETUP      (COMTP_CFG_SERVICE_ID_SETUP)
    /** BOOTLOADER_CMD -> 0b1010 */
    #define APP_CFG_BOOTLDR_SERVICE_ID_CMD      (0x03)
    /** Flash Page Buffer */
    #define APP_CFG_PAGE_BUFFER_SIZE            (FLASH_CFG_PAGE_SIZE)

    #define APP_CFG_BLDR_CMD_ERASE              (0xA0)
    #define APP_CFG_BLDR_CMD_WRITE              (0xA1)
    #define APP_CFG_BLDR_CMD_READ               (0xA2)
    #define APP_CFG_BLDR_CMD_RESET              (0xA3)

    #define APP_CFG_BLDR_STATE_READY            (0x00)
    #define APP_CFG_BLDR_STATE_BUSY             (0x01)

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

    /**
     * Service callback to be setup on the ComM:
     * This service handles requests to bootloader commands.
     * ComM is enabled only when App is in Reprogramming state.
     */
    extern void App_ComM_Service_Cmd( ComM_Frame_t * );

#endif
