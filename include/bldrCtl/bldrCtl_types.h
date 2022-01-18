#ifndef _BLDR_CTL_TYPES_H_
#define	_BLDR_CTL_TYPES_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include <stdint.h>
    
    typedef enum BldrStdReturn_t{
        BLDR_OK,
        BLDR_NOT_OK
    }BldrStdReturn_t;

    typedef enum BldrState_t{
        BLDR_UNINIT,
        BLDR_INIT,
        BLDR_IDLE,
        BLDR_BUSY
    }BldrState_t;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */