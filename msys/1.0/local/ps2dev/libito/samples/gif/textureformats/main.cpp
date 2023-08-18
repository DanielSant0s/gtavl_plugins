#include <Ito.h>

using namespace Ito;

int main()
{
	GifPipe	Gif;	
	Texture texture[12];

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
	
	Gif.SetupEnv(RGBA32, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FIELD, ENABLE, ScreenX, ScreenY);

	

	// Load IIF files from host
	Gif.LoadImageFile(texture[0], "host:rgba32.iif");
	Gif.LoadImageFile(texture[1], "host:rgb24.iif");
	Gif.LoadImageFile(texture[2], "host:rgba16.iif");
	
	Gif.LoadImageFile(texture[3], "host:clut8rgba32.iif");
	Gif.LoadImageFile(texture[4], "host:clut8rgba16.iif");
	Gif.LoadImageFile(texture[5], "host:clut4rgba32.iif");
	Gif.LoadImageFile(texture[6], "host:clut4rgba16.iif");

	// Load same textures again, this time they are just swizzled which results
	// in better upload speed from ee to gs
	Gif.LoadImageFile(texture[7], "host:s-rgba16.iif");
	Gif.LoadImageFile(texture[8], "host:s-clut8rgba32.iif");
	Gif.LoadImageFile(texture[9], "host:s-clut8rgba16.iif");
	Gif.LoadImageFile(texture[10], "host:s-clut4rgba32.iif");
	Gif.LoadImageFile(texture[11], "host:s-clut4rgba16.iif");

	texture[1].Dump();
	texture[2].Dump();

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		
		Gif.ClearScreen();
		
		// Textures are show in the same order they are loaded
		
		for(u32 i = 0; i < 4; i++) Gif.TextureSprite(texture[i], 0, i*150, 0);
		for(u32 i = 0; i < 3; i++) Gif.TextureSprite(texture[i+4], 0, i*150, 80);
	
		// Swizzled textures
		for(u32 i = 0; i < 4; i++) Gif.TextureSprite(texture[i+7], 0, i*150, 200);
		for(u32 i = 0; i < 1; i++) Gif.TextureSprite(texture[i+7+4], 0, i*150, 280);

		Gif.Send();
	}


	return 0;
}
