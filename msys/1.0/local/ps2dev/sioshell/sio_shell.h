////////////////////////////////////////////////////////////////////////////////
// sio_shell.h - Header file for the SIO shell
// Written by TyRaNiD (c) 2005
// Much props to Herben und Pixel
////////////////////////////////////////////////////////////////////////////////

#ifndef __SIO_SHELL_H__
#define __SIO_SHELL_H__

#include <kernel.h>

typedef union _mips_reg
{
	u128 r128;
	u64  r64[2]; 
	u32  r32[4];
} mips_reg;

struct ee_regs
{
	mips_reg r[32];
	mips_reg pmfhi;
	mips_reg pmflo;
	u32      BadVAddr;
	u32      Status;
	u32      Cause;
	u32      EPC;
	u32		 ErrorPC;
	u32      PerfCnt;
	u32      SA;
};

/* Define the syscalls */
struct syscall_table
{
	void (*Invalid0)(void);								/* 0 */
	void (*ResetEE)(u32 init_bitfield);
	void (*SetGsCrt)(s16 interlace, s16 pal_ntsc, s16 field);
	void (*Invalid3)(void);
	void (*Exit)(s32 exit_code);
	void (*RFU005)(void);
	void (*LoadExecPS2)(const char *filename, s32 num_args, char *args[]);
	s32  (*ExecPS2)(void *entry, void *gp, int num_args, char *args[]);
	void (*Invalid8)(void);
	void (*RFU009)(u32 arg0, u32 arg1);
	s32  (*AddSbusIntcHandler)(s32 cause, void (*handler)(int call)); /* 10 */
	s32  (*RemoveSbusIntcHandler)(s32 cause);
	s32  (*Interrupt2Iop)(s32 cause);
	void (*SetVTLBRefillHandler)(s32 handler_num, void* handler_func);
	void (*SetVCommonHandler)(s32 handler_num, void* handler_func);
	void (*SetVInterruptHandler)(s32 handler_num, void* handler_func);
	s32  (*AddIntcHandler)(s32 cause, s32(*handler_func)(s32 cause), s32 next);
	s32  (*RemoveIntcHandler)(s32 cause, s32 handler_id);
	s32	 (*AddDmacHandler)(s32 channel, s32 (*handler)(s32 channel), s32 next);
	s32	 (*RemoveDmacHandler)(s32 channel, s32 handler_id);
	s32  (*EnableIntc)(s32 cause); /* 20 */
	s32  (*DisableIntc)(s32 cause);
	s32  (*EnableDmac)(s32 channel);
	s32  (*DisableDmac)(s32 channel);
	s32  (*SetAlarm)(u16 time, void (*callback)(s32 alarm_id, u16 time, void *arg2), void *arg2);
	s32  (*ReleaseAlarm)(s32 alarm_id);
	s32  (*iEnableIntc)(s32 cause);
	s32  (*iDisableIntc)(s32 cause);
	s32  (*iEnableDmac)(s32 channel);
	s32  (*iDisableDmac)(s32 channel);
	s32  (*iSetAlarm)(u16 time, void (*callback)(s32 alarm_id, u16 time, void *arg2), void *arg2); /* 30 */
	s32  (*iReleaseAlarm)(s32 alarm_id);
	s32	 (*CreateThread)(ee_thread_t *thread);
	s32	 (*DeleteThread)(s32 thread_id);
	s32	 (*StartThread)(s32 thread_id, void *args);
	void (*ExitThread)(void);
	void (*ExitDeleteThread)(void);
	s32  (*TerminateThread)(s32 thread_id);
	s32  (*iTerminateThread)(s32 thread_id);
	void (*DisableDispatchThread)(void);	// not supported
	void (*EnableDispatchThread)(void);		/* 40 */
	s32  (*ChangeThreadPriority)(s32 thread_id, s32 priority);
	s32  (*iChangeThreadPriority)(s32 thread_id, s32 priority);
	s32  (*RotateThreadReadyQueue)(s32 priority);
	s32  (*iRotateThreadReadyQueue)(s32 priority);
	s32  (*ReleaseWaitThread)(s32 thread_id);
	s32  (*iReleaseWaitThread)(s32 thread_id);
	s32	 (*GetThreadId)(void);
	s32  (*ReferThreadStatus)(s32 thread_id, ee_thread_t *info);
	s32  (*iReferThreadStatus)(s32 thread_id, ee_thread_t *info);
	s32  (*SleepThread)(void); /* 50 */
	s32	 (*WakeupThread)(s32 thread_id);
	s32	 (*iWakeupThread)(s32 thread_id);
	s32	 (*CancelWakeupThread)(s32 thread_id);
	s32	 (*iCancelWakeupThread)(s32 thread_id);
	s32	 (*SuspendThread)(s32 thread_id);
	s32	 (*iSuspendThread)(s32 thread_id);
	s32	 (*ResumeThread)(s32 thread_id);
	s32	 (*iResumeThread)(s32 thread_id);
	u8 	 (*JoinThread)(void);
	void* (*SetupThread)(void * gp, void * stack, s32 stack_size, void * args, void * root_func); /* 60 */
	void  (*SetupHeap)(void * heap_start, s32 heap_size);
	void* (*EndOfHeap)(void);
	void (*Invalid63)(void);
	s32	 (*CreateSema)(ee_sema_t *sema);
	s32	 (*DeleteSema)(s32 sema_id);
	s32	 (*SignalSema)(s32 sema_id);
	s32	 (*iSignalSema)(s32 sema_id);
	s32	 (*WaitSema)(s32 sema_id);
	s32	 (*PollSema)(s32 sema_id);
	s32	 (*iPollSema)(s32 sema_id); /* 70 */
	s32	 (*ReferSemaStatus)(s32 sema_id, ee_sema_t *sema);
	s32	 (*iReferSemaStatus)(s32 sema_id, ee_sema_t *sema);
	s32	 (*iDeleteSema)(s32 sema_id);
	void (*SetOsdConfigParam)(void* addr);
	void (*GetOsdConfigParam)(void* addr);
	void (*GetGsHParam)(void* addr1, void* addr2, void* addr3);
	s32  (*GetGsVParam)(void);
	void (*SetGsHParam)(void* addr1, void* addr2, void* addr3, void* addr4);
	void (*SetGsVParam)(s32 arg1);
	void (*CreateEventFlag)(void); // Unsupported ? 80
	void (*DeleteEventFlag)(void);
	void (*SetEventFlag)(void);
	void (*iSetEventFlag)(void);
	void (*Invalid84)(void);
	void (*Invalid85)(void);
	void (*Invalid86)(void);
	void (*Invalid87)(void);
	void (*Invalid88)(void);
	void (*Invalid89)(void);
	void (*Invalid90)(void);	/* 90 */
	void (*Invalid91)(void);
	void (*EnableIntcHandler)(u32 cause);
	void (*DisableIntcHandler)(u32 cause);
	void (*EnableDmacHandler)(u32 cause);
	void (*DisableDmacHandler)(u32 cause);
	void (*KSeg0)(s32 arg1);
	s32  (*EnableCache)(s32 cache);
	s32  (*DisableCache)(s32 cache);
	u32  (*GetCop0)(s32 reg_id);
	void (*Unknown100)(u32 arg1);	/* 100 */
	void (*FlushCache)(s32 operation);
	u32  (*CpuConfig)(u32 config);
	u32  (*iGetCop0)(s32 reg_id);
	void (*Unknown104)(u32 arg1);
	void (*iFlushCache)(s32 operation);
	u32  (*iCpuConfig)(u32 config);
	void (*SifStopDma)(void);
	void (*SetCPUTimerHandler)(void (*handler)(void));
	void (*SetCPUTimer)(s32 compval);
	void (*SetOsdConfigParam2)(void* config, s32 size, s32 offset); /* 110 */
	void (*GetOsdConfigParam2)(void* config, s32 size, s32 offset);
	u64  (*GsGetIMR)(void);
	u64  (*GsPutIMR)(u64 imr);
	void (*SetPgifHandler)(void* handler);
	void (*SetVSyncFlag)(u32 *, u64 *);
	void (*SetSyscall)(s32 syscall_num, void* handler);
	void (*print)(void); // Null function
	s32  (*SifDmaStat)(u32 id);
	u32  (*SifSetDma)(SifDmaTransfer_t *sdd, s32 len);
	void (*SifSetDChain)(void);	/* 120 */
	int  (*SifSetReg)(u32 register_num, int register_value);
	int  (*SifGetReg)(u32 register_num);
	void (*ExecOSD)(int num_args, char *args[]);
	void (*Invalid124)(void);
	void (*PSMode)(void);
	s32  (*MachineType)(void);
	s32  (*GetMemorySize)(void);
};

typedef void (*printf_type)(const char *, ...);
extern printf_type g_sio_printf;

// prevent accidental printf'ing
#define printf g_sio_printf

extern struct syscall_table *_syscallTable;
extern struct syscall_table *_syscallTableOld;

/* Flush instruction and data caches */
void flush_cache(void);

/* Restart the level1 syscall handler */
void restartSyscallException(void);

#endif
