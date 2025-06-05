#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

//**********      General helper macros and type definitions     **********//

#include <inttypes.h>

#define FALSE 0
#define TRUE 1

#define wSWAP(v)			((((v) >> 8) & 0x00FF) | (((v) << 8) & 0xFF00))
#define GET_MSB(x)			(((uint16_t) x) >> 8)
#define GET_LSB(x)			(((uint16_t) x) & 0xff)
#define MSB(uint16_t)		(((uint8_t* )&uint16_t)[1])
#define LSB(uint16_t)		(((uint8_t* )&uint16_t)[0])


#endif /*TYPEDEFS_H_*/
