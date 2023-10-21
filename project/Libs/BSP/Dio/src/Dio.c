#include "Dio.h"
#include "Dio_types.h"
#include "Dio_include.h"

void Dio_Init( void ){
    /* Nothing to do here ... */
}

void Dio_Open( void ){
    PORTD = 0x00;
    TRISD = 0x00;
    LATD = DIO_INITIAL_OUTPUT;
}

DioLevel_t Dio_ReadChannel( DioChannel_t channel ){
    DioLevel_t retVal = Dio_INVALID;
    switch( channel ){
            case Dio_ch_0:
                    retVal = (DioLevel_t) PORTDbits.RD0;
                    break;
            case Dio_ch_1:
                    retVal = (DioLevel_t) PORTDbits.RD1;
                    break;
            case Dio_ch_2:
                    retVal = (DioLevel_t) PORTDbits.RD2;
                    break;
            case Dio_ch_3:
                    retVal = (DioLevel_t) PORTDbits.RD3;
                    break;
            case Dio_ch_4:
                    retVal = (DioLevel_t) PORTDbits.RD4;
                    break;
            case Dio_ch_5:
                    retVal = (DioLevel_t) PORTDbits.RD5;
                    break;
            case Dio_ch_6:
                    retVal = (DioLevel_t) PORTDbits.RD6;
                    break;
            case Dio_ch_7:
                    retVal = (DioLevel_t) PORTDbits.RD7;
                    break;
            default:
                    break;
    }
    return retVal;
}

void Dio_WriteChannel( DioChannel_t channel , DioLevel_t level ){
    switch( channel ){
            case Dio_ch_0:
                    LATDbits.LD0 = level;
                    break;
            case Dio_ch_1:
                    LATDbits.LD1 = level;
                    break;
            case Dio_ch_2:
                    LATDbits.LD2 = level;
                    break;
            case Dio_ch_3:
                    LATDbits.LD3 = level;
                    break;
            case Dio_ch_4:
                    LATDbits.LD4 = level;
                    break;
            case Dio_ch_5:
                    LATDbits.LD5 = level;
                    break;
            case Dio_ch_6:
                    LATDbits.LD6 = level;
                    break;
            case Dio_ch_7:
                    LATDbits.LD7 = level;
                    break;
            default:
                    break;
    }
}

