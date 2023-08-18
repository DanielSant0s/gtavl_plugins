#include <Gs.h>
#include <GsDefs.h>
#include <stdio.h>

using namespace Ito;


#ifdef F_GetPixelSize
u32 Gs::GetPixelSize(s32 psm)
{
	switch(psm) 
	{	
		case RGBA32: return 4; break;
		case RGB24:  return 3; break;
		case RGBA16: return 2; break;
		case CLUT8:	 return 1; break;
		default: return 0;
	}
}	
#endif

#ifdef F_GetZPixelSize	
u32 Gs::GetZPixelSize(s32 mode)
{
	switch(mode) 
	{
		case ZBUF32:	return 4; break;
		case ZBUF24:	return 3; break;
		case ZBUF16:	return 2; break;
		case ZBUF16S:	return 2; break;
		default: return 0;
	}
}
#endif


#ifdef F_GetTextureSize
u32 Gs::GetTextureSize(u32 psm, u32 width, u32 height)
{
	if(psm == CLUT4) 
		return ((width*height)/2);
	else
		return (width*height*GetPixelSize(psm));	
}
#endif

#ifdef F_SetBGCOLOR
static u64 GsRegisterBGCOLOR;

void Gs::SetBGCOLOR(u64 color)
{
	GS_REG_BGCOLOR = GsRegisterBGCOLOR = color;
}

u64 Gs::GetBGCOLOR()
{
	return GsRegisterBGCOLOR;
}
#endif

#ifdef F_SetSMODE2
static u64 GsRegisterSMODE2;

void Gs::SetSMODE2(u32 interlace, u32 ffmode, u32 vesa)
{
	GS_REG_SMODE2 = GsRegisterSMODE2 = ((u64)(vesa) << 2) | ((u64)(ffmode) << 1) | (interlace);
}

u64 GetSMODE2()
{
	return GsRegisterSMODE2;
}
#endif

#ifdef F_SetPMODE
static u64 GsRegisterPMODE;

void Gs::SetPMODE(u32 rc1, u32 rc2, u32 mmod, u32 amod, u32 slbg, u32 alpha)
{
	GS_REG_PMODE =  GsRegisterPMODE = ((u64)(alpha << 8) | ((u64)slbg << 7) | ((u64)amod << 6) | ((u64)mmod << 5) | ((u64)0x1 << 2) | ((u64)rc2) << 1) | rc1;
}

u64 GetPMODE()
{
	return GsRegisterPMODE;
}
#endif

#ifdef F_SetDISPFB1
static u64 GsRegisterDISPFB1;

void Gs::SetDISPFB1(u32 fbp, u32 fbw, u32 psm, u32 dbx, u32 dby)
{
	GS_REG_DISPFB1 = GsRegisterDISPFB1 = ((u64)dby << 43) | ((u64)dbx << 32) | ((u64)psm << 15) | ((u64)(fbw/64) << 9) | (fbp/8192);
}

u64 GetDISPFB1()
{
	return GsRegisterDISPFB1;
}
#endif

#ifdef F_SetDISPFB2
static u64 GsRegisterDISPFB2;

void Gs::SetDISPFB2(u32 fbp, u32 fbw, u32 psm, u32 dbx, u32 dby)
{
	GS_REG_DISPFB2 = GsRegisterDISPFB2 = ((u64)dby << 43) | ((u64)dbx << 32) |((u64)psm << 15) | ((u64)(fbw/64) << 9) | (fbp/8192);
}

u64 GetDISPFB2()
{	
	return GsRegisterDISPFB2;
}
#endif

#ifdef F_SetDISPLAY1
static u64 GsRegisterDISPLAY1;

void Gs::SetDISPLAY1(u32 dx, u32 dy, u32 mag_x, u32 mag_y, u32 dw, u32 dh)
{
	GS_REG_DISPLAY1 = GsRegisterDISPLAY1 = (((u64)dh) << 44) | (((u64)dw) << 32) | (((u64)mag_y) << 27) | (((u64)mag_x) << 23) | (((u64)dy) << 12) | ((u64)dx);
}

u64 GetDISPLAY1()
{
	return GsRegisterDISPLAY1;
}
#endif

#ifdef F_SetDISPLAY2
static u64 GsRegisterDISPLAY2;

void Gs::SetDISPLAY2(u32 dx, u32 dy, u32 mag_x, u32 mag_y, u32 dw, u32 dh)
{
	GS_REG_DISPLAY2 = GsRegisterDISPLAY2 = (((u64)dh) << 44) | (((u64)dw) << 32) | (((u64)mag_y) << 27) | (((u64)mag_x) << 23) | (((u64)dy) << 12) | ((u64)dx);
}

u64 GetDISPLAY2()
{
	return GsRegisterDISPLAY2;
}
#endif



#ifdef F_Reset
void Gs::Reset()
{
	GS_REG_CSR = 0x200;
}
#endif

#ifdef F_GetVideoMode
u32 Gs::GetVideoMode()
{
	return VMODE_AUTO;
}
#endif

#ifdef F_PrintControlRegisters
void Gs::PrintControlRegisters()
{
	printf("GIF_CTRL  : 0x%08X\n", GIF_REG_CTRL);
	printf("GIF_MODE  : 0x%08X\n", GIF_REG_MODE);
	printf("GIF_STAT  : 0x%08X\n", GIF_REG_STAT);
	printf("GIF_TAG0  : 0x%08X\n", GIF_REG_TAG0);
	printf("GIF_TAG1  : 0x%08X\n", GIF_REG_TAG1);
	printf("GIF_TAG2  : 0x%08X\n", GIF_REG_TAG2);
	printf("GIF_TAG3  : 0x%08X\n", GIF_REG_TAG3);
	printf("GIF_CNT   : 0x%08X\n", GIF_REG_CNT);
	printf("GIF_P3CNT : 0x%08X\n", GIF_REG_P3CNT);
	printf("GIF_P3TAG : 0x%08X\n", GIF_REG_P3TAG);
}
#endif

#ifdef F_VSyncWait
void Gs::VSyncWait()
{
	GS_REG_CSR &= 8; // generate
	while(!(GS_REG_CSR & 8)); // wait until its generated
}
#endif

#ifdef F_TextureBufferBase
static u32 TextureBufferBase = 0;

void Gs::SetTextureBufferBase(u32 base)
{
	TextureBufferBase = base;
}

u32 Gs::GetTextureBufferBase()
{
	return TextureBufferBase;
}
#endif
