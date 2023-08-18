#include <tamtypes.h>
#include <kernel.h>

namespace Ito
{
	class Sema;
}

/** Wrapper class for semaphores. */
class Ito::Sema
{
	public:
		u32 m_SemaId;
		ee_sema_t m_Sema;
	public:
		Sema();
		~Sema();
		s32 Create(u32 initCount=0, u32 maxCount=1);
		s32 Delete();
		s32 iDelete();
		s32 Signal();
		s32 iSignal();
		s32 Poll();
		s32 iPoll();
		s32 Wait();
};
