#include <Ito.h>

using namespace Ito;

static GifPipe gif;


/** Make the functions C style instead of C++ style */
extern "C"
{
void itoGsReset()
{
	Gs::Reset();
}


u32 itoGsGetVideoMode()
{
	return Gs::GetVideoMode();
}

void itoGifSetupEnv(u32 psm, u32 width, u32 height, u32 bufferCount, u32 vmode, u32 ffmode, u32 interlace, u32 x, u32 y, u32 zpsm)
{
	gif.SetupEnv(psm, width, height, bufferCount, vmode, ffmode, interlace, x, y, zpsm);
}

void itoGifSetNextDrawbuffer()
{
	gif.SetNextDrawbuffer();
}

void itoGifSprite(u64 color, s32 x1, s32 y1, s32 x2, s32 y2, u32 z)
{
	gif.Sprite(color, x1, y1, x2, y2, z);
}

void itoGifClearScreen()
{
	gif.ClearScreen();
}

void itoGifSend()
{
	gif.Send();
}

} /* end extern "C" */
