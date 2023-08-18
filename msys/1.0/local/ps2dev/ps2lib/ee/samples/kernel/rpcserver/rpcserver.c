/*      
  _____     ___ ____ 
   ____|   |    ____|      PSX2 OpenSource Project
  |     ___|   |____       (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  rpcserver.c
                           EE-based RPC server.
*/

#include <tamtypes.h>
#include <ps2lib_err.h>
#include <kernel.h>
#include <sifrpc.h>

#define RPC_SERVER_ID		0x6d0000d6
#define THREAD_STACK_SIZE	(8 * 1024)

SifRpcDataQueue_t subcon_qd;
SifRpcServerData_t subcon_sd;
u8 subcon_rpcbuf[4096] ALIGNED(16);

void subcon_rpc_thread(void *arg);

int th_wait_sem = -1;
u8 thread_stack[THREAD_STACK_SIZE] ALIGNED(16);

void print_error(int err);

int main()
{
	ee_thread_t thread;
	ee_sema_t sema;
	int thid;
	extern void *_gp;

	init_scr();
	scr_printf("RPC Server demo.\n\n");

	/* Create a temporary semaphore that we'll use to wait for the RPC
	   thread to finish initializing.  */
	sema.init_count = 1;
	sema.max_count = 1;
	if ((th_wait_sem = CreateSema(&sema)) < 0) {
		print_error(th_wait_sem);
		goto done;
	}

	thread.func = subcon_rpc_thread;
	thread.stack = thread_stack;
	thread.stack_size = THREAD_STACK_SIZE;
	thread.gp_reg = _gp;
	thread.initial_priority = 42;

	if ((thid = CreateThread(&thread)) < 0) {
		print_error(thid);
		goto done;
	}

	StartThread(thid, NULL);
	WaitSema(th_wait_sem);
	DeleteSema(th_wait_sem);
	scr_printf("IOP messages:\n\n");

done:
	SleepThread();
}

void * subcon_dispatch(int func, void *buf, int size)
{
	scr_printf(buf);

	return buf;
}

void subcon_rpc_thread(void *arg)
{
	int thid;

	scr_printf("RPC thread: Started.\n");
	thid = GetThreadId();

	SifInitRpc(0);
	SifSetRpcQueue(&subcon_qd, thid);
	SifRegisterRpc(&subcon_sd, RPC_SERVER_ID, subcon_dispatch,
			subcon_rpcbuf, NULL, NULL, &subcon_qd);

	scr_printf("RPC thread: Ready to take requests.\n");
	SignalSema(th_wait_sem);
	SifRpcLoop(&subcon_qd);

	/* SifRpcLoop() doesn't return. */
	while (1) ;
}

void print_error(int err)
{
}
