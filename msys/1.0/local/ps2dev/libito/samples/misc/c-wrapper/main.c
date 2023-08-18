#include "Gif.h"

/* Simple sample which shows how to use libito from C. */
int main()
{
	u32 ScreenWidth  = 640;
	u32 ScreenHeight = 448;
	u32 ScreenX = 155;
	u32 ScreenY = 70;
	u32 x = 0;

	if(itoGsGetVideoMode() == VMODE_PAL)  
	{
		ScreenHeight = 512;
		ScreenY = 70;
	}

	itoGsReset();
	
	itoGifSetupEnv(RGBA16, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FIELD, ENABLE, ScreenX, ScreenY, NOZBUF);

	while(1)
	{
		itoGifSetNextDrawbuffer();	
		itoGifClearScreen();	

		itoGifSprite(RGB(0xFF-x,0xFF-x,0xFF), 64 + x, 64, 128 + x, 128, 0);
		

		x = (x+4) % 255;

		itoGifSend();
	}


	return 0;
}
