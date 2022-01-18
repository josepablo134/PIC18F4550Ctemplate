#ifndef _FLS_CTL_TYPES_H_
#define	_FLS_CTL_TYPES_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    #include <stdint.h>

    typedef uint16_t FlashAddress_t;

    typedef uint16_t FlashSize_t;

    typedef enum FlashStdReturn_t{
        FLS_OK,
        FLS_NOT_OK
    }FlashStdReturn_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
