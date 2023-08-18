#include <tamtypes.h>
#include <kernel.h>

namespace Ito
{
	class Thread;
}

/** Wrapper class for threads */
class Ito::Thread
{
	public:
		s32 m_ThreadId;
		ee_thread_t m_Thread;
		u8* m_Stack;
		u32 m_StackSize;
	protected:
		s32 Sleep();
		void Exit();
		void ExitDelete();
	public:
		Thread();
		~Thread();
		s32 Create(void* function, u32 priority = 40);
		s32 Start(void* args = 0);
		s32 Suspend();
		s32 iSuspend();
		s32 Resume();
		s32 iResume();
		s32 Wakeup();
		s32 iWakeup();
		s32 Delete();
		s32 ChangePriority(u32 priority);
		s32 iChangePriority(u32 priority);
		s32 Terminate();
		s32 iTerminate();
};

