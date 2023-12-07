#ifndef STDINT_H__
#define STDINT_H__

#ifndef NULL
#define NULL	(void *)0
#endif

#define bool        _Bool
#define true        1
#define false       0

typedef unsigned int size_t;

typedef char   int8_t;
typedef short  int16_t;
typedef int  int32_t;
typedef long int  int64_t;
typedef __int128_t int128_t;
typedef int intptr_t;

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef unsigned long int  uint64_t;
typedef __uint128_t uint128_t;
typedef unsigned int uintptr_t;

#endif /* stdint.h */
