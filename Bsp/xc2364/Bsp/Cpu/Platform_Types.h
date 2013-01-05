#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/* The Types below are basic types for 16 bits or 32 bits cpu */
typedef signed char    sint8_t ;
typedef unsigned char  uint8_t ;
typedef signed short   sint16_t;
typedef unsigned short uint16_t;
typedef signed long    sint32_t;
typedef unsigned long  uint32_t;

/* The types below are Cpu relevent */
typedef signed int     sbase_t ;
typedef unsigned int   ubase_t ;

/* void type for 16bit banked cpu should be 32 bit */
typedef uint32_t       void_t;

#endif /* _PLATFORM_TYPES_H_ */
