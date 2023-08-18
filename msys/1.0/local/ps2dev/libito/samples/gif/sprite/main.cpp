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

	u32 x = 0;

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		
		Gif.ClearScreen();
		Gif.Sprite(RGB(0xFF-x,0xFF-x,0xFF), 64 + x, 64, 128 + x, 128);
		

		x = (x+4) % 255;
	
		Gif.Send();
	}


	return 0;
}
