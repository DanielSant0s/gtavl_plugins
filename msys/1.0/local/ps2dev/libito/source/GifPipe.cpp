#include <GifPipe.h>
#include <GifPacket.h>
#include <GsDefs.h>
#include <Gs.h>
#include <DmaDefs.h>
#include <TextureManager.h>
#include <stdio.h>
#include <kernel.h>
#include <malloc.h>

using namespace Ito;

#define GIFPIPE_PACKET_MAX_LEFT		(8*16) // If there is less than this value space left in a m_CurPacketacket, it gets sent.
#define GIFPIPE_PACKET_MAX_SIZE		DMA_MAX_SIZE-64

void GifAddVRHandler(GifPipe *pipe);
void GifRemoveVRHandler();

// Vertical Retrace Handler
#ifdef F_GifVRHandler
static u32 VRCount = 0;
static GifPipe* VRPipe = 0;
static s32 VRHandle = 0;
TextureManager *TM = 0;

s32 GifVRHandler(s32 cause)
{
	if(VRPipe != 0)
	{	
		VRCount++;

		if(VRPipe->m_FieldsPrFrame <= VRCount)
		{
			if(VRPipe->ShowNextFramebuffer())
			{
				TM->Tick();
				VRCount = 0;
			}
			
		}
	}

	EIntr();
	
	return 0;
}


void GifAddVRHandler(GifPipe *pipe)
{
	if(VRPipe == 0)
	{
		VRPipe = pipe;
		TM = TextureManager::Instance();

		DI();
		VRHandle = AddIntcHandler(2, GifVRHandler, 0);
		EnableIntc(2);
		EI();
	}
}

void GifRemoveVRHandler()
{
	VRPipe = 0;

	DI();
	DisableIntc(2);
	RemoveIntcHandler(2, VRHandle);
	EI();

}
#endif

#ifdef F_Constructor
GifPipe::GifPipe() 
{
	m_RegisterTEX0 = 0;
	m_RegisterTEST = 0;
	m_RegisterZBUF = 0;

	m_GifPackets[0] = new GifPacket();
	m_GifPackets[1] = new GifPacket();

	m_GifPackets[0]->SetContext(m_Context);
	m_GifPackets[1]->SetContext(m_Context);
	
	if(m_Circuit == 1)
		m_PrimMode = PRIM_CONTEXT_1;
	else
		m_PrimMode = PRIM_CONTEXT_2;

	m_CurPacket = m_GifPackets[0];
	m_CurPacket->Reset();

	GifAddVRHandler(this);
}
#endif

#ifdef F_Constructor2
GifPipe::GifPipe(u32 size)
{
	
}
#endif

#ifdef F_Destructor
GifPipe::~GifPipe()
{
	delete m_GifPackets[0];
	delete m_GifPackets[1];
	GifRemoveVRHandler();
}
#endif

#ifdef F_SetNextDrawBuffer
void GifPipe::SetNextDrawbuffer()
{
	while(m_UsedFramebuffers == m_FramebufferCount);
	
	m_UsedFramebuffers++;
	
	m_ActiveFramebuffer = (m_ActiveFramebuffer+1) % m_FramebufferCount;
	
	SetRegister(GS_REG_FRAME, GS_FRAME(m_ActiveFramebuffer*m_FramebufferSize, m_FramebufferWidth, m_FramebufferPSM, 0));

}
#endif

#ifdef F_CheckPacket
void GifPipe::CheckPacket()
{
	if(m_CurPacket->GetUsedSize() > (GIFPIPE_PACKET_MAX_SIZE - GIFPIPE_PACKET_MAX_LEFT))
	{
		//printf("Check packet!\n");
		Send();
	}

//	printf("GOGO!");
}
#endif

#ifdef F_Send
void GifPipe::Send()
{
	m_CurPacket->End();
	m_CurPacket->Send();
	
	if(m_CurPacket == m_GifPackets[0])
		m_CurPacket = m_GifPackets[1];
	else
		m_CurPacket = m_GifPackets[0];

	m_CurPacket->Reset();
}
#endif

#ifdef F_Finish
extern u32 vr_test;
void GifPipe::Finish()
{
	Send();
	m_CurPacket->Wait();
}
#endif


#ifdef F_UploadTexture
bool GifPipe::UploadTexture(Texture& texture)
{
	bool res = m_CurPacket->AddUploadTexture(texture);

	CheckPacket();

	return res;
}
#endif

#ifdef F_LoadImageBuffer
bool GifPipe::LoadImageBuffer(Texture& texture, u8* buffer)
{
	if(texture.LoadImageBuffer(buffer) == false)
		return false;
	
	if(UploadTexture(texture) == false)
		return false;
	
	return true;
}
#endif

#ifdef F_LoadImageFile
bool GifPipe::LoadImageFile(Texture& texture, char* filename)
{
	if(texture.LoadImageFile(filename) == false)
		return false;
	
	if(UploadTexture(texture) == false)
		return false;

	return true;
}
#endif

#ifdef F_PrimShading
void GifPipe::PrimShading(u32 shading)
{
	m_PrimMode &= ~PRIM_GOURAUD;
	m_PrimMode |= shading;
}
#endif

#ifdef F_PrimAlphaBlend
void GifPipe::PrimAlphaBlend(bool enable)
{
	if(enable)
		m_PrimMode |= PRIM_ALPHABLEND;
	else
		m_PrimMode &= ~PRIM_ALPHABLEND;
}
#endif

#ifdef F_PrimAntialias
void GifPipe::PrimAntialias(bool enable)
{
	if(enable)
		m_PrimMode |= PRIM_ANTIALIAS;
	else
		m_PrimMode &= ~PRIM_ANTIALIAS;

}
#endif

/*
#ifdef F_PrimMode
void GifPipe::PrimMode(u32 mode)
{
	m_PrimMode = mode & (PRIM_GOURAUD|PRIM_TEXTURE|PRIM_FOG|PRIM_ALPHABLEND|PRIM_ANTIALIAS|PRIM_UV);
}
#endif
*/

#ifdef F_ClearScreen
void GifPipe::ClearScreen(u64 color)
{
	Sprite(color, 0, 0, m_FramebufferWidth, m_FramebufferHeight, 0);
}
#endif

#ifdef F_SetRenderTarget2
void GifPipe::SetRenderTarget(GsTexture* texture)
{
	SetRenderTarget(texture->tbp*256, texture->psm, texture->width, texture->height);
}
#endif

#ifdef F_SetRenderTarget
void GifPipe::SetRenderTarget(u32 base, u32 psm, u32 width, u32 height)
{
	// Set frame & scissor register, calc temp prim_xy

}
#endif


#ifdef F_RestoreRenderTarget
void GifPipe::RestoreRenderTarget()
{
	// Reset frame & scissor register

}
#endif


#ifdef F_SetRegister
void GifPipe::SetRegister(u64 address, u64 data)
{
	m_CurPacket->AddSetRegister(address, data);
	CheckPacket();
}
#endif

#ifdef F_AlphaBlending2
bool GifPipe::AlphaBlending(u32 mode)
{
	u32 a=0,b=0,c=0,d=0,fix=0;

	switch(mode)
	{
		case AlphaBlendNormal:
			a = ALPHA_COLOR_SRC;
			b = ALPHA_COLOR_DST;
			c = ALPHA_VALUE_SRC;
			d = ALPHA_COLOR_DST;
		break;

		case AlphaBlendAdditive:
			a = ALPHA_COLOR_SRC;
			b = ALPHA_ZERO;
			c = ALPHA_FIXED;
			d = ALPHA_COLOR_DST;
			fix = 0x80;
		break;

		case AlphaBlendSubtractive:
			a = ALPHA_COLOR_DST;
			b = ALPHA_COLOR_SRC;
			c = ALPHA_FIXED;
			d = ALPHA_ZERO;
			fix = 0x80;
		break;
		
		case AlphaBlendLighten:
			a = ALPHA_COLOR_DST;
			b = ALPHA_ZERO;
			c = ALPHA_VALUE_SRC;
			d = ALPHA_COLOR_DST;
		break;

		case AlphaBlendDarken:
			a = ALPHA_ZERO;	
			b = ALPHA_COLOR_DST;
			c = ALPHA_VALUE_SRC;
			d = ALPHA_COLOR_DST;
		break;

		default:
			return false;
	}

	SetRegister(GS_REG_ALPHA, GS_ALPHA(a, b, c, d, fix)); 

	return true;
}
#endif


#ifdef F_AlphaBlending 
void GifPipe::AlphaBlending(u32 a, u32 b, u32 c, u32 d, u8 fix)
{
	SetRegister(GS_REG_ALPHA, GS_ALPHA(a, b, c, d, fix)); 
}
#endif

#ifdef F_AlphaTest
void GifPipe::AlphaTest(bool enable, u32 atst, u8 aref, u32 afail, u32 date, u32 datm)
{
	m_RegisterTEST &= ~0xFFFF;
	m_RegisterTEST |= GS_TEST( enable, atst, aref, afail, date, datm, 0, 0);
	
	SetRegister(GS_REG_TEST, m_RegisterTEST);
}
#endif

#ifdef F_ZTest
void GifPipe::ZTest(u32 enable, u32 ztst)
{
	m_RegisterTEST &= 0xFFFF; 

	if(enable)
	{
	//	printf("Enable\n");
		m_RegisterTEST |= GS_TEST( 0, 0, 0, 0, 0, 0, 1, ztst);
		m_RegisterZBUF &= ~0x100000000;
	}
	else
	{
		m_RegisterTEST |= GS_TEST( 0, 0, 0, 0, 0, 0, 1, ZTEST_ALWAYS);
	//	m_RegisterZBUF |= 0x100000000;
	}

//	printf("GS_REG %i\n", GS_REG_TEST);

	SetRegister(GS_REG_TEST, m_RegisterTEST);
	SetRegister(GS_REG_ZBUF, m_RegisterZBUF);
}
#endif

#ifdef F_SetTexture
bool GifPipe::SetTexture(Texture& texture)
{
	UploadTexture(texture);

	//m_RegisterTEX0 &= ~0x3FFFFFFFF;


	if(!texture.HasClut())
		m_RegisterTEX0 = GS_TEX0(texture.Base(), texture.tbw, texture.psm, texture.tw, texture.th, 0, 1, 0, 0, 0, 0, 0);
	else
		m_RegisterTEX0 = GS_TEX0(texture.Base(), texture.tbw, texture.Psm(), texture.tw, texture.th, 0, 1, texture.clut->Base(), texture.clut->Csm(), 0, 0, 1);

	SetRegister(GS_REG_TEX0, m_RegisterTEX0);
/*

	#define GS_TEX0(tbp0, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld) \
			(tbp0/256) | (s64)((tbw)/64) << 14 | (s64)(psm) << 20 \
			| (s64)(tw) << 26 | (s64)(th) << 30 | (s64)(tcc) << 34 \
			| (s64)(tfx) << 35 | (s64)((cbp) / 256) << 37 | (s64)(cpsm) << 51 \
			| (s64)(csm) << 55 | (s64)((csa)/16) << 56 | (s64)(cld) << 61 
*/
/*
	u32 tbp0 = (u32)(m_RegisterTEX0 & 0x3FFF);
	u32 tbw = (u32)((m_RegisterTEX0 >> 14) & 0x3F);
	u32 psm = (u32)((m_RegisterTEX0 >> 20) & 0x3F);

	printf("TEX0(0x%08X, %i, %i)\n", tbp0<<8, tbw*64, psm);
*/
	return true;
}
#endif

#ifdef F_ColorClamp
void GifPipe::ColorClamp(u32 mode)
{
	
}
#endif

#ifdef F_TextureWrap
void GifPipe::TextureWrap(u32 wm_s, u32 wm_t, u32 min_u, u32 max_u, u32 min_v, u32 max_v)
{

}
#endif

#ifdef F_Dither
void GifPipe::Dither(u32 enable, DitherMatrix *matrix)
{

	if(enable & (u32)matrix)
	{	
		for(s32 y=0; y < 4; y++)
		{
			for(int x=0; x < 4; x++)
			{		
				matrix[x][y] = (matrix[x][y] & 0x3) | ((matrix[x][y] >> 5) & 0x4);
			}
		}
	}
}
#endif

// ----------------------------------------------------------------------------
// Primitives
// ----------------------------------------------------------------------------

#ifdef F_Sprite4
void GifPipe::Sprite4(u64 color, s32 x1, s32 y1, s32 x2, s32 y2, u32 z)
{
	m_CurPacket->Giftag(	1, ENABLE, DISABLE, 0, GIFTAG_LIST, 4, 
					GIFTAG_REGS(GIFTAG_PRIM, GIFTAG_RGBAQ, GIFTAG_XYZ3, GIFTAG_XYZ2));
	
	m_CurPacket->AddDWord(GS_PRIM2(PRIM_SPRITE, PRIM_SHADE_FLAT, DISABLE, m_PrimMode));
	m_CurPacket->AddDWord(GS_RGBAQ(color));
	m_CurPacket->AddDWord(GS_XYZ3(x1, y1, z)+m_PrimOffsetXY);
	m_CurPacket->AddDWord(GS_XYZ2(x2, y2, z)+m_PrimOffsetXY);

	CheckPacket();
}
#endif

#ifdef F_Line4
void GifPipe::Line4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2)
{
	m_CurPacket->Giftag(	1, ENABLE, DISABLE, 0, GIFTAG_LIST, 6, 
					GIFTAG_REGS(GIFTAG_PRIM, GIFTAG_RGBAQ, GIFTAG_XYZ3, GIFTAG_RGBAQ, GIFTAG_XYZ2, GIFTAG_NOP));
	
	m_CurPacket->AddDWord(GS_PRIM2(PRIM_LINE, 0, DISABLE, m_PrimMode));
	
	m_CurPacket->AddDWord(GS_RGBAQ(color1));
	m_CurPacket->AddDWord(GS_XYZ3(x1, y1, z1)+m_PrimOffsetXY);
	
	m_CurPacket->AddDWord(GS_RGBAQ(color2));
	m_CurPacket->AddDWord(GS_XYZ2(x2, y2, z2)+m_PrimOffsetXY);
	
	m_CurPacket->AddDWord(GS_NOP);

	CheckPacket();
}
#endif

#ifdef F_Triangle4
void GifPipe::Triangle4(u64 color1, s32 x1, s32 y1, u32 z1, u64 color2, s32 x2, s32 y2, u32 z2, u64 color3, s32 x3, s32 y3, u32 z3)
{
	m_CurPacket->Giftag(	1, ENABLE, DISABLE, 0, GIFTAG_LIST, 8, 
					GIFTAG_REGS(GIFTAG_PRIM, GIFTAG_RGBAQ, GIFTAG_XYZ3, GIFTAG_RGBAQ, GIFTAG_XYZ3, GIFTAG_RGBAQ, GIFTAG_XYZ2, GIFTAG_NOP));
	
	m_CurPacket->AddDWord(GS_PRIM2(PRIM_TRIANGLE, 0, DISABLE, m_PrimMode));
	
	m_CurPacket->AddDWord(GS_RGBAQ(color1));
	m_CurPacket->AddDWord(GS_XYZ3(x1, y1, z1)+m_PrimOffsetXY);
	
	m_CurPacket->AddDWord(GS_RGBAQ(color2));
	m_CurPacket->AddDWord(GS_XYZ3(x2, y2, z2)+m_PrimOffsetXY);
	
	m_CurPacket->AddDWord(GS_RGBAQ(color3));
	m_CurPacket->AddDWord(GS_XYZ2(x3, y3, z3)+m_PrimOffsetXY);
	
	m_CurPacket->AddDWord(GS_NOP);

	CheckPacket();
}
#endif


#ifdef F_Point4 
void GifPipe::Point4(u64 color, s32 x, s32 y, s32 z)
{
	m_CurPacket->Giftag(	1, ENABLE, DISABLE, 0, GIFTAG_LIST, 4, 
					GIFTAG_REGS(GIFTAG_PRIM, GIFTAG_RGBAQ, GIFTAG_XYZ2, GIFTAG_NOP));
	
	m_CurPacket->AddDWord(GS_PRIM2(PRIM_POINT, PRIM_SHADE_FLAT, DISABLE, m_PrimMode));
	
	m_CurPacket->AddDWord(GS_RGBAQ(color));
	m_CurPacket->AddDWord(GS_XYZ2(x, y, z)+m_PrimOffsetXY);
	m_CurPacket->AddDWord(GS_NOP);

	CheckPacket();
}
#endif

#ifdef F_TextureSprite
void GifPipe::TextureSprite4(u64 color, s32 x1, s32 y1, u32 u1, u32 v1, s32 x2, s32 y2, u32 u2, u32 v2, u32 z)
{
	m_CurPacket->Giftag(	1, ENABLE, DISABLE, 0, GIFTAG_LIST, 6, 
					GIFTAG_REGS(GIFTAG_PRIM, GIFTAG_RGBAQ, GIFTAG_UV, GIFTAG_XYZ3, GIFTAG_UV, GIFTAG_XYZ2));
	
	m_CurPacket->AddDWord(GS_PRIM2(PRIM_SPRITE, PRIM_SHADE_FLAT, ENABLE, m_PrimMode | PRIM_UV));
	m_CurPacket->AddDWord(GS_RGBAQ(color));
	m_CurPacket->AddDWord(GS_UV(u1, v1));
	m_CurPacket->AddDWord(GS_XYZ3(x1, y1, z)+m_PrimOffsetXY);
	m_CurPacket->AddDWord(GS_UV(u2, v2));
	m_CurPacket->AddDWord(GS_XYZ2(x2, y2, z)+m_PrimOffsetXY);
	
	CheckPacket();
}
#endif

#ifdef F_TextureSprite2
void GifPipe::TextureSprite4(Texture& texture, u64 color, u32 x1, u32 y1, u32 x2, u32 y2, u32 z)
{
	if(texture.addr == 0) return;

	SetTexture(texture);

	if((x2 == 0xFFFF0) && (y2 == 0xFFFF0))
	{
		x2 = x1 + (texture.width << 4);
		y2 = y1 + (texture.height << 4);
	}	

	TextureSprite4(color, x1, y1, 0, 0, x2, y2, texture.width << 4, texture.height << 4, z);

}
#endif




