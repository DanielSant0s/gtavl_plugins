#include <Thread.h>
#include <malloc.h>
#include <string.h>

using namespace Ito;

#ifdef F_Constructor
Thread::Thread()
{
	memset(&m_Thread, 0, sizeof(ee_thread_t));
	m_ThreadId = 0;
}
#endif

#ifdef F_Destructor
Thread::~Thread()
{
	if(m_Stack) free(m_Stack);
}
#endif

#ifdef F_Create
extern void* _gp;
s32 Thread::Create(void* function, u32 priority)
{
	m_StackSize = 1024;
	m_Stack = (u8*)memalign(64, 1024);
	
	m_Thread.func = function;
	m_Thread.stack = m_Stack;
	m_Thread.stack_size = m_StackSize;
	m_Thread.gp_reg = _gp;
	m_Thread.initial_priority = priority;

	m_ThreadId = CreateThread(&m_Thread);

	return m_ThreadId;
}
#endif

#ifdef F_Start
s32 Thread::Start(void* args)
{
	return StartThread(m_ThreadId, args);
}
#endif

#ifdef F_Suspend
s32 Thread::Suspend()
{
	return SuspendThread(m_ThreadId);
}
#endif

#ifdef F_iSuspend
s32 Thread::iSuspend()
{
	return iSuspendThread(m_ThreadId);
}
#endif

#ifdef F_Resume
s32 Thread::Resume()
{
	return ResumeThread(m_ThreadId);
}
#endif

#ifdef F_iResume
s32 Thread::iResume()
{
	return iResumeThread(m_ThreadId);
}
#endif


#ifdef F_Wakeup
s32 Thread::Wakeup()
{
	return WakeupThread(m_ThreadId);
}
#endif

#ifdef F_iWakeup
s32 Thread::iWakeup()
{
	return iWakeupThread(m_ThreadId);
}
#endif

#ifdef F_Sleep
s32 Thread::Sleep()
{
	return SleepThread();
}
#endif

#ifdef F_Delete
s32 Thread::Delete()
{
	return DeleteThread(m_ThreadId);
}
#endif

#ifdef F_ChangePriority
s32 Thread::ChangePriority(u32 priority)
{
	return ChangeThreadPriority(m_ThreadId, priority);
}
#endif

#ifdef F_iChangePriority
s32 Thread::ChangePriority(u32 priority)
{
	return iChangeThreadPriority(m_ThreadId, priority);
}
#endif

#ifdef F_Terminate
s32 Thread::Terminate()
{
	return TerminateThread(m_ThreadId);
}
#endif

#ifdef F_iTerminate
s32 Thread::iTerminate()
{
	return iTerminateThread(m_ThreadId);
}
#endif

#ifdef F_Exit
void Thread::Exit()
{
	ExitThread();
}
#endif

#ifdef F_ExitDelete
void Thread::ExitDelete()
{
	ExitDeleteThread();
}
#endif

