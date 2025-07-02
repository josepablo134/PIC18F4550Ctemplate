#include "App.h"

#define APP_RST_VECT        0x002000
#define APP_HISR            0x002008
#define APP_LISR            0x002018

static uint8_t     bldr_flash_page_buffer[APP_CFG_PAGE_BUFFER_SIZE];
static uint8_t     bldr_state;
static uint8_t     bldr_size;
static uint16_t    bldr_addr;

void App_Init( void ){
    Board_Init();
    Scheduler_Init();
    UART_Init();
    UART_OpenStatic();
}

App_state_t App_mainFunction( App_state_t state ){
    switch ( state )
    {
        case APP_STATE_INIT:
            state = App_State_Init( state );
            break;
        case APP_STATE_APP_CHECK:
            state = App_State_AppCheck( state );
            break;
        case APP_STATE_APP_REPROGRAMMING:
            state = App_State_AppReprogramming( state );
            break;
        case APP_STATE_APP_SAFE_STATE:
            state = App_State_AppSafeState( state );
            break;
        default:
            break;
    }
    return state;
}

App_state_t App_State_Init( App_state_t state ){
    if( XMODEM_OK == xModem_waitAck( XMODEM_TRUE ) ){
        xModem_Ack();
        state = APP_STATE_APP_REPROGRAMMING;
    }else{
        state = APP_STATE_APP_CHECK;
    }
    return state;
}

App_state_t App_State_AppCheck( App_state_t state ){
    STKPTR = 0U;
    asm("GOTO 0x2000");
    return state;
}

App_state_t App_State_AppReprogramming( App_state_t state ){
    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;

    while( 1U ){
        UART_putch( UART_getch() );
        LATAbits.LA4 = !PORTAbits.RA4;
    }

    LATAbits.LA4 = 0U;
    return state;
}

App_state_t App_State_AppSafeState( App_state_t state ){
    sleep();
    /** Make PortA full DIO */
    TRISA = 0x00;
    PORTA = 0x00;
    LATA = 0x00;
    ADCON1 = 0x0F;
    while (1)
    {
        LATAbits.LA4 = !PORTAbits.RA4;
        __delay_ms( 100U );
    }
    return state;
}
