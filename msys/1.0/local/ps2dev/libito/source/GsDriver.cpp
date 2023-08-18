#include <GsDriver.h>
#include <GifPacket.h>
#include <GsDefs.h>
#include <Gs.h>
#include <Ito.h>
#include <kernel.h>
#include <stdio.h>

using namespace Ito;

#define GIFPACKET_SIZE	(32*16)

#ifdef F_Constructor
GsDriver::GsDriver() 
{
	m_GifPacket = new GifPacket();
	
	m_Circuit	= 2;
	m_Context	= 2;
}
#endif

#ifdef F_Destructor
GsDriver::~GsDriver()
{
	delete m_GifPacket;	
}
#endif

#ifdef F_SetupEnv
//s32 GsDriver::Init(s32 width, s32 height, s32 psm, s32 nbuffers, s32 vmode, s32 ffmode, s32 interlace, s32 x, s32 y, s32 zpsm, s32 context, s32 circuit)
bool GsDriver::SetupEnv(u32 psm, u32 width, u32 height, u32 bufferCount, u32 vmode, u32 ffmode, u32 interlace, u32 x, u32 y, u32 zpsm)
{
	// Return -1 on arg error
	if(bufferCount < 1) return false;
	
	if(ffmode == FIELD) 
		m_FieldsPrFrame = 2;
	else
		m_FieldsPrFrame = 1;

	m_FramebufferWidth		= width;
	m_FramebufferHeight		= height;
	m_FramebufferPSM		= psm;
	m_FramebufferCount		= bufferCount;
	m_VisibleFramebuffer	= 0;
	m_ActiveFramebuffer		= 0;
	m_UsedFramebuffers		= 0;
	m_FramebufferSize		= Gs::GetTextureSize(psm, width, height);

	// Framebuffers must be on a 8192 byte align.
	if(m_FramebufferSize % 8192)	
		m_FramebufferSize += 8192 - (m_FramebufferSize % 8192);
	
	u32 screen_offset_x	= ((4096-width)/2)  << 4;
	u32 screen_offset_y	= ((4096-height)/2) << 4;
	
	// We can just add this value to XYZn reg.
	m_PrimOffsetXY			= (screen_offset_y << 16) | screen_offset_x;
	
	// The display_n register needs to be set, requires a bit of calculating
	u32 display_mag_x		= (2560/width)-1; // Width must be about 2560 in display_width
	u32 display_mag_y		= 0; 
	u32 display_width		= (width*(display_mag_x+1))-1; // in vck units
	u32 display_x			= x*(display_mag_x+1);
	u32 display_y			= y;
	u32 display_height;

	// Special frame + interlace mode
	if( (ffmode == FRAME) && (interlace == ENABLE) )
		display_height = (height * 2)-1;
	else
		display_height = height-1;

	GsPutIMR(0xFF00);
	SetGsCrt(interlace, vmode , ffmode);
	
	Gs::SetPMODE(DISABLE, ENABLE, 0, 0, 0, 0xFF);
	Gs::SetDISPFB2(0, m_FramebufferWidth, m_FramebufferPSM, 0, 0);
	Gs::SetDISPLAY2(display_x, display_y, display_mag_x, display_mag_y, display_width, display_height);

	// Setup frambuffers
	m_GifPacket->SetContext(m_Context);
	m_GifPacket->Reset();
	
	if(zpsm != NOZBUF)
	{
		Gs::SetTextureBufferBase((m_FramebufferCount*m_FramebufferSize)+m_FramebufferWidth*m_FramebufferHeight*Gs::GetZPixelSize(zpsm));

		m_RegisterZBUF = GS_ZBUF(m_FramebufferCount*m_FramebufferSize, zpsm, 0);
		
		m_GifPacket->Giftag(5, ENABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
		m_GifPacket->AddAD(GS_REG_ZBUF,	m_RegisterZBUF);
	}	
	else
	{
		// Set Global Texture Buffer base
		Gs::SetTextureBufferBase(m_FramebufferCount*m_FramebufferSize);
	
		m_GifPacket->Giftag(4, ENABLE, DISABLE, 0, GIFTAG_PACKED, 1, GIFTAG_AD);
	}
	
	m_GifPacket->AddAD(GS_REG_FRAME,		GS_FRAME(0, m_FramebufferWidth, m_FramebufferPSM, 0));
	m_GifPacket->AddAD(GS_REG_XYOFFSET,		GS_XYOFFSET(screen_offset_x, screen_offset_y));
	m_GifPacket->AddAD(GS_REG_SCISSOR,		GS_SCISSOR(0, 0, width-1, height-1));
	m_GifPacket->AddAD(GS_REG_PRMODECONT,	GS_PRMODECONT(GS_PRMODECONT_PRIM));	
	m_GifPacket->End();
	
	//m_GifPacket->Dump();
	
	m_GifPacket->Send();
	m_GifPacket->Wait();

	return true;
}
#endif

#define GIFTAG_IMAGE_MAX_QWORD		(0x7FF0 * 16)

#ifdef F_UploadTexture
bool GsDriver::UploadTexture(Texture& texture)
{
	if(texture.IsAllocated() == false)
	{
		if(texture.Allocate() == false)
			return false;
	}

	m_GifPacket->Reset();
	m_GifPacket->StartChain();
	m_GifPacket->AddUploadTextureChain(texture);
	m_GifPacket->EndChain();
	m_GifPacket->Send();

	m_GifPacket->Wait();

	return true;
}
#endif

#ifdef F_SetNextDrawBuffer
void GsDriver::SetNextDrawbuffer()
{
	// Wait until a framebuffer is avaliable to avoid overwriting
	// previous frames

	while(m_UsedFramebuffers == m_FramebufferCount);
	
	m_UsedFramebuffers++;
	
	m_ActiveFramebuffer = (m_ActiveFramebuffer+1) % m_FramebufferCount;
	
	SetRegister(GS_REG_FRAME, GS_FRAME(m_ActiveFramebuffer*m_FramebufferSize, m_FramebufferWidth, m_FramebufferPSM, 0));

}
#endif

#ifdef F_ShowNextFramebuffer
bool GsDriver::ShowNextFramebuffer()
{
	if(m_UsedFramebuffers > 0) 
	{
		m_UsedFramebuffers--;
		m_VisibleFramebuffer = (m_VisibleFramebuffer+1) % m_FramebufferCount;

		Gs::SetDISPFB2(m_VisibleFramebuffer*m_FramebufferSize, m_FramebufferWidth, m_FramebufferPSM, 0, 0);
		
		return true;
	}

	return false;
}
#endif

#ifdef F_CircuitBlend
s32 GsDriver::CircuitBlend(bool enable, s32 alpha)
{

	return 0;
}
#endif

