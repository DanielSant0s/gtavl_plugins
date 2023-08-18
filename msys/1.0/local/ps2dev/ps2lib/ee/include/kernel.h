/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (C)2001, Gustavo Scotti (gustavo@scotti.com)
  ------------------------------------------------------------------------
  kernel.h
			EE Kernel prototypes
                        These are my findings based on debug-info elf files.
*/

#ifndef _KERNEL_H
#define _KERNEL_H

#include <stddef.h>
#include <stdarg.h>

#include "sifdma.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DI	{				\
      		u32 cop0;			\
   do {						\
      __asm__(".p2align 3");			\
      __asm__(" di ");				\
      __asm__(" sync.p"); 			\
      asm volatile(" mfc0 %0, $12" : "=r"(cop0):);	\
      } while (cop0 & 0x10000);			\
   }

#define EI					\
      __asm__(" ei ")

#define EE_SYNC __asm__ volatile ("sync")

#define UNCACHED_SEG(x)		\
   (void *)((u32)x | 0x20000000)

#define IS_UNCACHED_SEG(x)		\
   ((u32)x & 0x20000000)

#define PUSHDATA( t, x, v, l) \
    *(t *)x = v; l = sizeof( t)

#define POPDATA( t, x, v, l) \
    v = *(t *)x; l = sizeof( t)

#define ALIGNED(x) __attribute__((aligned((x))))

static inline void nopdelay()
{
	int i = 0xfffff;

	do {
		asm ("nop\nnop\nnop\nnop\nnop\n");
	} while (i-- != -1);
}

typedef struct t_ee_sema
{
	int   count,
	      max_count,
	      init_count,
	      wait_threads;
	u32   attr,
	      option;
} ee_sema_t;

typedef struct t_ee_thread
{
	int   status;
	void* func;
	void* stack;
	int   stack_size;
	void* gp_reg;
	int   initial_priority;
	int   current_priority;
	u32   attr;
	u32   option;
} ee_thread_t;

enum _sif_regs {
	SIF_REG_MAINADDR = 1,
	SIF_REG_SUBADDR,
	SIF_REG_MSFLAG,
	SIF_REG_SMFLAG,
};

/* System call prototypes */
void ResetEE(u32 init_bitfield);
void SetGsCrt(s16 interlace, s16 pal_ntsc, s16 field);
void Exit(s32 exit_code);
void LoadExecPS2(const char *filename, s32 num_args, char *args[]);
s32  ExecPS2(void *entry, void *gp, int num_args, char *args[]);
void RFU009(u32 arg0, u32 arg1);
s32  AddSbusIntcHandler(s32 cause, void*(handler)(int call));
s32  RemoveSbusIntcHandler(s32 cause);
s32  Interrupt2Iop(s32 cause);
void SetVTLBRefillHandler(s32 handler_num, void* handler_func);
void SetVCommonHandler(s32 handler_num, void* handler_func);
void SetVInterruptHandler(s32 handler_num, void* handler_func);
s32  AddIntcHandler(s32 cause, s32(*handler_func)(s32 cause), s32 next);
s32  AddIntcHandler2(s32 cause, s32(*handler_func)(s32 cause, void* arg, void* addr), s32 next, void* arg);
s32  RemoveIntcHandler(s32 cause, s32 handler_id);
s32	 AddDmacHandler(s32 channel, s32 (*handler)(s32 channel), s32 next);
s32	 RemoveDmacHandler(s32 channel, s32 handler_id);
s32  EnableIntc(s32 cause);
int  _EnableIntc(int);
s32  DisableIntc(s32 cause);
int  _DisableIntc(int);
s32	 EnableDmac(s32 channel);
int  _EnableDmac(int);
s32	 DisableDmac(s32 channel);
int  _DisableDmac(int);
s32  SetAlarm(u16 time, void (*callback)(s32 alarm_id, u16 time, void *arg2), void *arg2);
s32  ReleaseAlarm(s32 alarm_id);
s32  iEnableIntc(s32 cause);
s32  iDisableIntc(s32 cause);
s32  iEnableDmac(s32 channel);
s32  iDisableDmac(s32 channel);
s32  iSetAlarm(u16 time, void (*callback)(s32 alarm_id, u16 time, void *arg2), void *arg2);
s32  iReleaseAlarm(s32 alarm_id);
s32	 CreateThread(ee_thread_t *thread);
s32	 DeleteThread(s32 thread_id);
s32	 StartThread(s32 thread_id, void *args);
void ExitThread(void);
void ExitDeleteThread(void);
s32  TerminateThread(s32 thread_id);
s32  iTerminateThread(s32 thread_id);
// void DisableDispatchThread(void);	// not supported
// void EnableDispatchThread(void);		// not supported
s32  ChangeThreadPriority(s32 thread_id, s32 priority);
s32  iChangeThreadPriority(s32 thread_id, s32 priority);
s32  RotateThreadReadyQueue(s32 priority);
s32  iRotateThreadReadyQueue(s32 priority);
s32  ReleaseWaitThread(s32 thread_id);
s32  iReleaseWaitThread(s32 thread_id);
s32	 GetThreadId(void);
s32  ReferThreadStatus(s32 thread_id, ee_thread_t *info);
s32  iReferThreadStatus(s32 thread_id, ee_thread_t *info);
s32  SleepThread(void);
s32	 WakeupThread(s32 thread_id);
s32	 iWakeupThread(s32 thread_id);
s32	 CancelWakeupThread(s32 thread_id);
s32	 iCancelWakeupThread(s32 thread_id);
s32	 SuspendThread(s32 thread_id);
s32	 iSuspendThread(s32 thread_id);
s32	 ResumeThread(s32 thread_id);
s32	 iResumeThread(s32 thread_id);
s32  JoinThread(void);
s32  RFU060(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void RFU061(s32 arg0, s32 arg1);
void* EndOfHeap(void);
s32	 CreateSema(ee_sema_t *sema);
s32	 DeleteSema(s32 sema_id);
s32	 SignalSema(s32 sema_id);
s32	 iSignalSema(s32 sema_id);
s32	 WaitSema(s32 sema_id);
s32	 PollSema(s32 sema_id);
s32	 iPollSema(s32 sema_id);
s32	 ReferSemaStatus(s32 sema_id, ee_sema_t *sema);
s32	 iReferSemaStatus(s32 sema_id, ee_sema_t *sema);
s32	iDeleteSema(s32 sema_id);
void SetOsdConfigParam(void* addr);
void GetOsdConfigParam(void* addr);
void GetGsHParam(void* addr1, void* addr2, void* addr3);
s32  GetGsVParam(void);
void SetGsHParam(void* addr1, void* addr2, void* addr3, void* addr4);
void SetGsVParam(s32 arg1);
void EnableIntcHandler(u32 arg1);
void iEnableIntcHandler(u32 arg1);
void DisableIntcHandler(u32 arg1);
void iDisableIntcHandler(u32 arg1);
void EnableDmacHandler(u32 arg1);
void iEnableDmacHandler(u32 arg1);
void DisableDmacHandler(u32 arg1);
void iDisableDmacHandler(u32 arg1);
void KSeg0(s32 arg1);
s32  EnableCache(s32 cache);
s32  DisableCache(s32 cache);
u32  GetCop0(s32 reg_id);
void FlushCache(s32 operation);
u32  CpuConfig(u32 config);
u32  iGetCop0(s32 reg_id);
void iFlushCache(s32 operation);
u32  iCpuConfig(u32 config);
void SetCPUTimerHandler(void (*handler)(void));
void SetCPUTimer(s32 compval);
void SetOsdConfigParam2(void* config, s32 size, s32 offset);
void GetOsdConfigParam2(void* config, s32 size, s32 offset);
u64  GsGetIMR(void);
u64  iGsGetIMR(void);
u64  GsPutIMR(u64 imr);
u64  iGsPutIMR(u64 imr);
void SetPgifHandler(void* handler);
void SetVSyncFlag(s32 arg1, s32 arg2);
void SetSyscall(s32 syscall_num, void* handler);
//void _print(const char *fmt, ...);		// null function

void SifStopDma(void);
s32  SifDmaStat(u32 id);
s32  iSifDmaStat(u32 id);
u32  SifSetDma(SifDmaTransfer_t *sdd, s32 len);
u32  iSifSetDma(SifDmaTransfer_t *sdd, s32 len);
void SifSetDChain(void);
void iSifSetDChain(void);
int  SifSetReg(u32 register_num, int register_value);
int  SifGetReg(u32 register_num);

void ExecOSD(int num_args, char *args[]);
s32  Deci2Call(s32 , u32 *);
void PSMode(void);
s32  MachineType(void);
s32  GetMemorySize(void);

void SifWriteBackDCache(void *ptr, int size);

/* C library string routines (EE-optimized) */
void *	memchr(const void *, int, size_t);
int	memcmp(const void *, const void *, size_t);
void *	memcpy(void *, const void *, size_t);
void *	memmove(void *, const void *, size_t);
void *	memset(void *, int, size_t);

char *	strcat(char *, const char *);
char *	strchr(const char *, int);
int	strcmp(const char *, const char *);
char *	strcpy(char *, const char *);
u32	strlen(const char *);
char *	strncat(char *, const char *, size_t);
int	strncmp(const char *, const char *, size_t);
char *	strncpy(char *, const char *, size_t);

/* stdio */
int	printf(const char *format, ...);
int	sprintf(char *str, const char *format, ...);
int	snprintf(char *str, size_t sz, const char *format, ...);

int	vprintf(const char *format, va_list args);
int	vsprintf(char *str, const char *format, va_list ap);
int	vsnprintf(char *str, size_t sz, const char *format, va_list args);

/* stdlib/malloc */
void *	malloc(size_t size);
void *	realloc(void *ptr, size_t size);
void *	calloc(size_t n, size_t size);
void *	memalign(size_t align, size_t size);
void	free(void * ptr);

/* You should never need to use this normally.  */
void *	ps2_sbrk(size_t incr);

/* stdlib - program termination */
void	abort(void);
void	exit(int retval);
void	_exit(int retval);

/* errno.h */
#ifndef errno
extern int errno;
int *__errno(void);
#endif

/* Napalm/Naplink puts() and printf().  */
int	npmPuts(const char *buf);
int	nprintf(const char *format, ...);

/* Screen output routines.  NOTE: these are going away VERY soon... */
void	init_scr(void);
void	scr_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif	// _KERNEL_H

