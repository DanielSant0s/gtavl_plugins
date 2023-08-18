/*
------------------------------------------------------------------------------
Standard definitions and types, Bob Jenkins
------------------------------------------------------------------------------
*/
#ifndef STANDARD
# define STANDARD

#include "wrappers.h"

#define UB8MAXVAL 0xffffffffffffffffLL
#define UB8BITS 64
#define SB8MAXVAL 0x7fffffffffffffffLL
#define UB4MAXVAL 0xffffffff
#define UB4BITS 32
#define SB4MAXVAL 0x7fffffff
#define UB2MAXVAL 0xffff
#define UB2BITS 16
#define SB2MAXVAL 0x7fff
#define UB1MAXVAL 0xff
#define UB1BITS 8
#define SB1MAXVAL 0x7f

#include <tamtypes.h>
typedef u64 ub8;
typedef s64 sb8;
typedef u32 ub4;
typedef s32 sb4;
typedef u16 ub2;
typedef s16 sb2;
typedef u8  ub1;
typedef s8  sb1;
typedef int  word;  /* fastest type available */

#define bis(target,mask)  ((target) |=  (mask))
#define bic(target,mask)  ((target) &= ~(mask))
#define bit(target,mask)  ((target) &   (mask))
#ifndef min
# define min(a,b) (((a)<(b)) ? (a) : (b))
#endif /* min */
#ifndef max
# define max(a,b) (((a)<(b)) ? (b) : (a))
#endif /* max */
#ifndef align
# define align(a) (((ub4)a+(sizeof(void *)-1))&(~(sizeof(void *)-1)))
#endif /* align */
#ifndef abs
# define abs(a)   (((a)>0) ? (a) : -(a))
#endif
#define TRUE  1
#define FALSE 0
#define SUCCESS 0  /* 1 on VAX */

#endif /* STANDARD */
