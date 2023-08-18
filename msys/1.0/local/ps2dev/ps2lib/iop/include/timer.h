/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2003, Nick Van Veen (nickvv@xtra.co.nz)
  ------------------------------------------------------------------------
  timer.h
			IOP timer related prototypes
*/

#ifndef _TIMER_H
#define _TIMER_H

struct timestamp
{
  int clk;
  int cnt;
};

int CancelAlarm(u32 (*handler)(void*), void *common); 
int iCancelAlarm(u32 (*handler)(void*), void *common); 

int GetSystemTime(struct timestamp *time);

int SetAlarm(struct timestamp *time, u32 (*handler)(void*), void *common);
int iSetAlarm(struct timestamp *time, u32 (*handler)(void*), void *common);

void SysClock2USec(struct timestamp *time, int *sec, int *usec);
void USec2SysClock(u32 usec, struct timestamp *time);

#endif
