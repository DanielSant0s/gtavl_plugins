/*
  _____     ___ ____
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  intrman.h
                           IOP intrman library prototypes

  Additions and changes from original function declarations in kernel.h
  by Florin Sasu.
*/

#ifndef __INTRMAN_H__
#define __INTRMAN_H__

#define INTRMAN_VER	0x102

int RegisterIntrHandler(int interrupt, int unknown, int (*intrh)(void*), void*);
int ReleaseIntrHandler( int interrupt );

int EnableIntr(int interrupt);	
int DisableIntr( int interrupt, int *oldstat );

int CpuDisableIntr();
void CpuEnableIntr();

int CpuSuspendIntr(int *oldstat);
void CpuResumeIntr(int oldstat);

int QueryIntrContext();	

#endif//__INTRMAN_H__
