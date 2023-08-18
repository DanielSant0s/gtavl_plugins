#include <Pad.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <malloc.h>

using namespace Ito;

#ifdef F_Constructor
static u32 LibpadInit = 0;
Pad::Pad(char* sio2manIrx, char* padmanIrx)
{
	if(!LibpadInit)
	{
		SifInitRpc(0);
		
		if(!sio2manIrx)
			sio2manIrx = "rom0:SIO2MAN";
		
		SifLoadModule(sio2manIrx, 0, NULL);
		
		if(!padmanIrx)
			padmanIrx = "rom0:PADMAN";	
	
		SifLoadModule(padmanIrx, 0, NULL);
	
		padInit(0);
	
		LibpadInit = 1;
	}

	m_PadBuffer = (u8*)memalign(64, 256);

	m_Slot = -1;
	m_Port = -1;
}
#endif


#ifdef F_Destructor
Pad::~Pad()
{
	free(m_PadBuffer);
}
#endif

#ifdef F_Open
s32 Pad::Open(s32 port, s32 slot)
{
	if((m_Port < 0) && (m_Slot < 0))
	{
		m_Port = port;
		m_Slot = slot;
	
		return padPortOpen(m_Port, m_Slot, m_PadBuffer);
	}

	return -1;
}
#endif

#ifdef F_Close
s32 Pad::Close()
{
	s32 slot = m_Slot;
	s32 port = m_Port;
	
	m_Slot = -1;
	m_Port = -1;

	return padPortClose(port, slot); 
}
#endif

#ifdef F_GetState
u32 Pad::GetState()
{
	return padGetState(m_Port, m_Slot);
}
#endif

#ifdef F_ReadButtons
u16 Pad::ReadButtons()
{
	s32 ret;
	
	ret = padGetState(m_Port, m_Slot);

	while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
	{
			if(ret == PAD_STATE_DISCONN) return 0;
			
            ret = padGetState(m_Port, m_Slot);
	}
	
	padRead(m_Port, m_Slot, &m_ButtonStatus);

	return 0xffff^m_ButtonStatus.btns;
}
#endif
