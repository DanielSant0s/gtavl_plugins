/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, Florin Sasu (florinsasu@yahoo.com)
  ------------------------------------------------------------------------
  sysmem.h
                           sysmem library prototypes
*/

#ifndef __SYSMEM_H__
#define __SYSMEM_H__

#define SYSMEM_VER	0x101

//allocation strategies
#define ALLOC_FIRST	0
#define ALLOC_LAST	1
#define ALLOC_LATER	2


// see QueryBlockTopAddress, QueryBlockSize
#define USED	0x00000000
#define FREE	0x80000000

//include <loadcore.h>
//extern struct export sysmem_stub;

void		*AllocSysMemory(int flags, int size, void *mem);
int		FreeSysMemory(void *mem);

unsigned int	QueryMemSize();
unsigned int	QueryMaxFreeMemSize();
unsigned int	QueryTotalFreeMemSize();

void		*QueryBlockTopAddress(void *address);
int		QueryBlockSize(void *address);

char		*Kprintf(const char *format,...);
void		SetKprintf(char* (*newKprintf)(unsigned int unk, const char*, ...), unsigned int newunk);

#endif//__SYSMEM_H__
