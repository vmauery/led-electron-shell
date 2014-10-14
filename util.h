#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// project/system dependent defines

// CPU clock speed
//#define F_CPU        16000000               		// 16MHz processor
//#define F_CPU        14745000               		// 14.745MHz processor
//#define F_CPU        12000000                     // 12MHz processor
//#define F_CPU        11059200                     // 11.0592MHz processor
//#define F_CPU        8000000               		// 8MHz processor
//#define F_CPU        7372800               		// 7.37MHz processor
//#define F_CPU        4000000               		// 4MHz processor
//#define F_CPU        3686400               		// 3.69MHz processor
#define F_CPU          1000000                      // 1MHz (internal osc) processor

#include <util/delay.h>

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond
#define UART_BAUD_RATE   19200
#define UART_BAUD_SEL    (F_CPU/(UART_BAUD_RATE*16L)-1)

typedef const PROGMEM uint8_t prog_uint8_t;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;
typedef signed int s16;
typedef unsigned long u32;
typedef signed long s32;

#ifndef MAX
#define MAX(A,B) (((A)>(B))?(A):(B))
#endif

#ifndef MIN
#define MIN(A,B) (((A)<(B))?(A):(B))
#endif

#ifndef ABS
#define ABS(x)				((x>0)?(x):(-x))
#endif

#define __ccat(a,b) a##b
#define _PORT(x)  __ccat(PORT, x)
#define _PIN(x)   __ccat(PIN, x)
#define _DDR(x)   __ccat(DDR, x)

// macro for reading 16bit words from program memory
#ifndef PRG_RDW
#define PRG_RDW(a)		( (PRG_RDB((unsigned char*)(a)) & 0x00ff) | ((PRG_RDB((unsigned char*)(a)+1))<<8) )
#endif

// constants
#define PI		3.14159265359

static inline void msleep(uint16_t msecs)
{
	while (msecs-- > 0) {
		_delay_ms(1);
	}
}

/*
 * atomic 16 bit register access
 */
#define get_reg_16_na(REG) \
({                      \
    uint8_t sreg;       \
    uint16_t ___i;      \
    sreg = SREG;        \
    __asm__ ("cli");    \
    ___i = REG##H;      \
	___i <<= 8;         \
    ___i |= REG##L;     \
    SREG = sreg;        \
    ___i;               \
})

#define get_reg_16(REG) \
({                      \
    uint8_t sreg;       \
    uint16_t ___i;      \
    sreg = SREG;        \
    __asm__ ("cli");    \
    ___i = REG;         \
    SREG = sreg;        \
    ___i;               \
})

#define set_reg_16_na(REG,VAL) \
do {                        \
    uint8_t sreg;           \
    sreg = SREG;            \
    __asm__ ("cli");        \
    REG##H = (VAL>>8);      \
    REG##L = (VAL&0xff);    \
    SREG = sreg;            \
} while(0)

#define set_reg_16(REG,VAL) \
do {                        \
    uint8_t sreg;           \
    sreg = SREG;            \
    __asm__ ("cli");        \
    REG = VAL;              \
    SREG = sreg;            \
} while(0)


#endif // UTIL_H
