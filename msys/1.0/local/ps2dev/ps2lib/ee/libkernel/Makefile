LIBKERNEL_LIB  = ../lib/libkernel.a
LIBSYSCALL_LIB = ../lib/libsyscall.a

# TODO: This is a hack ... it will be killed when new build system is
# installed.
OBJDIR = OBJ/
OBJEXIST = $(OBJDIR).created

SIFCMD_OBJS = _dmac_ctrl.o sif_cmd_send.o _sif_cmd_int_handler.o sif_cmd_main.o \
	sif_cmd_addhandler.o sif_sreg_get.o
SIFCMD_OBJS := $(SIFCMD_OBJS:%=$(OBJDIR)%)

SIFRPC_OBJS = SifBindRpc.o SifCallRpc.o SifRpcGetOtherData.o \
	SifRegisterRpc.o SifSetRpcQueue.o SifGetNextRequest.o \
	SifExecRequest.o SifRpcLoop.o SifRpcMain.o _rpc_get_packet.o \
	_rpc_get_fpacket.o SifCheckStatRpc.o
SIFRPC_OBJS := $(SIFRPC_OBJS:%=$(OBJDIR)%)

LIBSYSCALL_OBJS = ResetEE.o SetGsCrt.o Exit.o LoadExecPS2.o ExecPS2.o \
	RFU009.o AddSbusIntcHandler.o RemoveSbusIntcHandler.o Interrupt2Iop.o \
	SetVTLBRefillHandler.o SetVCommonHandler.o SetVInterruptHandler.o \
	AddIntcHandler.o AddIntcHandler2.o RemoveIntcHandler.o AddDmacHandler.o \
	RemoveDmacHandler.o _EnableIntc.o _DisableIntc.o _EnableDmac.o _DisableDmac.o \
	SetAlarm.o ReleaseAlarm.o iEnableIntc.o iDisableIntc.o iEnableDmac.o \
	iDisableDmac.o iSetAlarm.o iReleaseAlarm.o CreateThread.o DeleteThread.o \
	StartThread.o ExitThread.o ExitDeleteThread.o TerminateThread.o \
	iTerminateThread.o DisableDispatchThread.o EnableDispatchThread.o \
	ChangeThreadPriority.o iChangeThreadPriority.o RotateThreadReadyQueue.o \
	iRotateThreadReadyQueue.o ReleaseWaitThread.o iReleaseWaitThread.o \
	GetThreadId.o ReferThreadStatus.o iReferThreadStatus.o SleepThread.o \
	WakeupThread.o iWakeupThread.o CancelWakeupThread.o iCancelWakeupThread.o \
	SuspendThread.o iSuspendThread.o ResumeThread.o iResumeThread.o \
	JoinThread.o RFU060.o RFU061.o EndOfHeap.o CreateSema.o DeleteSema.o \
	iSignalSema.o SignalSema.o WaitSema.o PollSema.o iPollSema.o \
	ReferSemaStatus.o iReferSemaStatus.o iDeleteSema.o SetOsdConfigParam.o \
	GetOsdConfigParam.o GetGsHParam.o GetGsVParam.o SetGsHParam.o \
	SetGsVParam.o CreateEventFlag.o DeleteEventFlag.o SetEventFlag.o \
	iSetEventFlag.o EnableIntcHandler.o iEnableIntcHandler.o \
	DisableIntcHandler.o iDisableIntcHandler.o EnableDmacHandler.o \
	iEnableDmacHandler.o DisableDmacHandler.o iDisableDmacHandler.o KSeg0.o \
	EnableCache.o DisableCache.o GetCop0.o FlushCache.o CpuConfig.o \
	iGetCop0.o iFlushCache.o RFU105.o iCpuConfig.o SifStopDma.o \
	SetCPUTimerHandler.o SetCPUTimer.o SetOsdConfigParam2.o \
	GetOsdConfigParam2.o GsGetIMR.o iGsGetIMR.o GsPutIMR.o iGsPutIMR.o \
	SetPgifHandler.o SetVSyncFlag.o SetSyscall.o SifDmaStat.o iSifDmaStat.o \
	SifSetDma.o iSifSetDma.o SifSetDChain.o iSifSetDChain.o SifSetReg.o \
	SifGetReg.o ExecOSD.o Deci2Call.o PSMode.o MachineType.o GetMemorySize.o \
	SifWriteBackDCache.o __errno.o errno.o
LIBSYSCALL_OBJS := $(LIBSYSCALL_OBJS:%=$(OBJDIR)%)
	
FILEIO_OBJS = fio_main.o fio_exit.o fio_open.o fio_close.o fio_read.o \
	fio_write.o fio_lseek.o fio_mkdir.o _fio_read_intr.o fio_getc.o fio_putc.o
FILEIO_OBJS := $(FILEIO_OBJS:%=$(OBJDIR)%)

LOADFILE_OBJS = SifLoadFileInit.o SifLoadFileExit.o _SifLoadModule.o \
	SifLoadModule.o SifLoadModuleEncrypted.o _SifLoadElfPart.o \
	SifLoadElfPart.o SifLoadElf.o SifLoadElfEncrypted.o SifIopSetVal.o \
	SifIopGetVal.o _SifLoadModuleBuffer.o SifLoadModuleBuffer.o
LOADFILE_OBJS := $(LOADFILE_OBJS:%=$(OBJDIR)%)

IOPHEAP_OBJS = SifInitIopHeap.o SifExitIopHeap.o SifAllocIopHeap.o \
	SifFreeIopHeap.o SifLoadIopHeap.o
IOPHEAP_OBJS := $(IOPHEAP_OBJS:%=$(OBJDIR)%)

IOPCONTROL_OBJS = SifIopReset.o SifResetIop.o SifIopIsAlive.o SifIopSync.o
IOPCONTROL_OBJS := $(IOPCONTROL_OBJS:%=$(OBJDIR)%)

CONFIG_OBJS = config.o
CONFIG_OBJS := $(CONFIG_OBJS:%=$(OBJDIR)%)

XPRINTF_OBJS = vsnprintf.o vsprintf.o vprintf.o printf.o sprintf.o snprintf.o \
	npmPuts.o nprintf.o
XPRINTF_OBJS := $(XPRINTF_OBJS:%=$(OBJDIR)%)

ALLOC_OBJS = malloc.o realloc.o calloc.o memalign.o free.o __builtin_alloc.o
ALLOC_OBJS := $(ALLOC_OBJS:%=$(OBJDIR)%)

DEAD_OBJS = font.o graph.o
CORE_OBJS = sbrk.o memchr.o memcpy.o memset.o strcat.o strcmp.o strlen.o \
	strncmp.o memcmp.o memmove.o strchr.o strcpy.o strncat.o \
	strncpy.o terminate.o

MULT_OBJS = $(SIFCMD_OBJS) $(SIFRPC_OBJS) $(LIBSYSCALL_OBJS) $(FILEIO_OBJS) \
	$(LOADFILE_OBJS) $(IOPHEAP_OBJS) $(IOPCONTROL_OBJS) $(CONFIG_OBJS) \
	$(XPRINTF_OBJS) $(ALLOC_OBJS)

all: $(OBJEXIST) $(LIBKERNEL_LIB) $(LIBSYSCALL_LIB)

$(LIBKERNEL_LIB): $(CORE_OBJS) $(MULT_OBJS) $(DEAD_OBJS)
	$(EE_AR) cru $(LIBKERNEL_LIB) $(CORE_OBJS) $(MULT_OBJS) $(DEAD_OBJS)

$(LIBSYSCALL_LIB): $(LIBSYSCALL_OBJS)
	$(EE_AR) cru $(LIBSYSCALL_LIB) $(LIBSYSCALL_OBJS)

$(OBJEXIST):
	@echo; echo "Creating object directory"
	test -d $(dir $@) || mkdir $(dir $@)
	touch $@


$(SIFCMD_OBJS): sifcmd.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(SIFRPC_OBJS): sifrpc.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(LIBSYSCALL_OBJS): kernel.S
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(FILEIO_OBJS): fileio.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(LOADFILE_OBJS): loadfile.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(CONFIG_OBJS): config.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(IOPHEAP_OBJS): iopheap.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(IOPCONTROL_OBJS): iopcontrol.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(XPRINTF_OBJS): xprintf.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

$(ALLOC_OBJS): alloc.c
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(OBJDIR)%=%) $< -c -o $@

clean:
	-rm *.o
	-rm -rf $(OBJDIR)

include ../../Makefile.pref
include ../../Makefile.eeglobal
