#include <Ito.h>

using namespace Ito;

int main()
{
	GifPipe	Gif;	

	u32 ScreenWidth  = 640;
	u32 ScreenHeight = 448;
	u32 ScreenX = 155;
	u32 ScreenY = 70;

	if(Gs::GetVideoMode() == VMODE_PAL)  
	{
		ScreenHeight = 512;
		ScreenY = 70;
	}


	Gs::Reset();
	
	Gif.SetupEnv(RGBA16, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FIELD, ENABLE, ScreenX, ScreenY);

	Gif.AlphaBlending(0);

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		
		Gif.PrimAlphaBlend(false);
		Gif.ClearScreen();
		Gif.PrimAlphaBlend(true);
		

		Gif.Sprite(RGBA(0,0,0xFF, 0x80), 64, 64, 128, 128);
	
		Gif.Sprite(RGBA(0xFF,0,0, 0x40), 96, 96, 160, 160);

		Gif.Send();
	}


	return 0;
}
