/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
  tamtypes.h
			Common used typedef
*/

#ifndef _TAMTYPES_H_
#define _TAMTYPES_H_ 1


typedef	unsigned char 		u8;
typedef unsigned short 		u16;
typedef unsigned int 		u32;
typedef unsigned long int	u64;

#ifdef _EE
typedef unsigned int		u128 __attribute__(( mode(TI) ));
#endif

typedef signed char 		s8;
typedef signed short 		s16;
typedef	signed int 			s32;
typedef signed long int		s64;

#ifdef _EE
typedef int			s128 __attribute__(( mode(TI) ));
#endif

#ifndef NULL
#define NULL	(void *)0
#endif

extern inline u8  _lb(u32 addr) { return *(volatile u8 *)addr; }
extern inline u16 _lh(u32 addr) { return *(volatile u16 *)addr; }
extern inline u32 _lw(u32 addr) { return *(volatile u32 *)addr; }
extern inline u64 _ld(u32 addr) { return *(volatile u64 *)addr; }

extern inline void _sb(u8 val, u32 addr) { *(volatile u8 *)addr = val; }
extern inline void _sh(u16 val, u32 addr) { *(volatile u16 *)addr = val; }
extern inline void _sw(u32 val, u32 addr) { *(volatile u32 *)addr = val; }
extern inline void _sd(u64 val, u32 addr) { *(volatile u64 *)addr = val; }

#ifdef _EE
extern inline u128 _lq(u32 addr) { return *(volatile u128 *)addr; }
extern inline void _sq(u128 val, u32 addr) { *(volatile u128 *)addr = val; }
#endif

#endif
