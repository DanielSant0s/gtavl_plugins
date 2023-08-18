/*
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2002, David Ryan (Oobles@hotmail.com)
  ------------------------------------------------------------------------
  thbase.h
			   IOP thbase library prototypes
*/

#ifndef __THBASE_H__
#define __THBASE_H__


#define AT_THFIFO 0
#define AT_THPRI  1
#define AT_SINGLE 0
#define AT_MULTI  2
#define AT_MSFIFO 0
#define AT_MSPRI  4

#define SA_THFIFO AT_THFIFO
#define SA_THPRI  AT_THPRI
#define SA_IHTHPRI 0x100

#define TH_C		0x02000000

struct t_thread
{
  u32 type;
  u32 unknown;
  void (*function)(void *);
  u32 stackSize;
  u32 priority;
};


int CreateThread( struct t_thread * t );  //returns thread_id
int DeleteThread( int thread_id );

int StartThread( int thread_id, void * arg );

int ExitThread();
int ExitDeleteThread(void);
int TerminateThread( int thread_id );
int iTerminateThread( int thread_id );

int ChangeThreadPriority( int thread_id, int priority );
int iChangeThreadPriority( int thread_id, int priority );

int GetThreadId( );
int SleepThread( );

int WakeupThread( int thread_id );
int iWakeupThread( int thread_id );
int ReleaseWaitThread(int thread_id);
int iReleaseWaitThread(int thread_id);


int DelayThread( int micosec );

void USec2SysClock();

int GetSystemStatusFlag();

#endif//__THBASE_H__

