#include <Sema.h>
#include <string.h>

using namespace Ito;

#ifdef F_Constructor
Sema::Sema()
{
	memset(&m_Sema, 0, sizeof(ee_sema_t));
	m_SemaId = 0;
}
#endif

#ifdef F_Destructor
Sema::~Sema()
{

}
#endif

#ifdef F_Create
s32 Sema::Create(u32 initCount, u32 maxCount)
{
	m_Sema.init_count = initCount;
	m_Sema.max_count = maxCount;
	
	m_SemaId = CreateSema(&m_Sema);

	return m_SemaId;
}
#endif

#ifdef F_Delete
s32 Sema::Delete()
{
	return DeleteSema(m_SemaId);
}
#endif

#ifdef F_iDelete
s32 Sema::iDelete()
{
	return iDeleteSema(m_SemaId);
}
#endif

#ifdef F_Signal
s32 Sema::Signal()
{
	return SignalSema(m_SemaId);
}
#endif

#ifdef F_iSignal
s32 Sema::iSignal()
{
	return iSignalSema(m_SemaId);
}
#endif

#ifdef F_Poll
s32 Sema::Poll()
{
	return PollSema(m_SemaId);
}
#endif

#ifdef F_iPoll
s32 Sema::iPoll()
{
	return iPollSema(m_SemaId);
}
#endif

#ifdef F_Wait
s32 Sema::Wait()
{
	return WaitSema(m_SemaId);
}
#endif
