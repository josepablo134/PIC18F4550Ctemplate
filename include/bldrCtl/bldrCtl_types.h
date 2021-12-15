#ifndef _BLDR_CTL_TYPES_H_
#define	_BLDR_CTL_TYPES_H_

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    #include <stdint.h>
    
    typedef uint32_t bldr_address;
    
    typedef uint32_t bldr_size;
    
    typedef uint8_t  bldr_byte;
    
    typedef enum bldr_state{
        bldr_init,
        bldr_idle,
        bldr_busy
    }bldr_state;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */