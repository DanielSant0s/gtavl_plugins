#include <Ito.h>
#include <stdlib.h>

extern u8 fire_clut[];

#define buffer_width	512
#define buffer_height	256

#define NCOLORS			252 // Number of unique colors in clut

// This buffer is actually just a texture that is generated
// on each frame and uploaded to VRAM and then mapped onto a sprite
static u8 buffer[buffer_height][buffer_width] ALIGN(16);

void GenerateFire();

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
	Gif.SetupEnv(RGBA32, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FIELD, ENABLE, ScreenX, ScreenY);


	Texture texture, clut;
	
	Gif.LoadImageBuffer(clut, fire_clut);

	texture.CreateTexture((void*)buffer, CLUT8, buffer_width, buffer_height, clut);

	texture.Allocate();

	texture.AlwaysUpload(true);

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		Gif.ClearScreen();
		
		GenerateFire();

		Gif.TextureSprite(texture, 0, 0, 0, ScreenWidth, ScreenHeight);
	
		Gif.Send();
	}

	return 0;
}

// Based on tutorial found at http://www.geocities.com/SiliconValley/Circuit/5530/fire.html
void GenerateFire()
{
	u32 x = 1, y = 0, i = 0;

	// Generate bottom line
	while(x < buffer_width)
	{
		y = (u32)(rand() % 16);
		
		if((x+y) > buffer_width) y = buffer_width-x;

		buffer[buffer_height-1][x] = (u32)(rand() % NCOLORS-10)+10;
	
		for(i=1; i < y; i++)
			buffer[buffer_height-1][x+y] = buffer[buffer_height-1][x+y-1];
	
		x+=y;
	}

	// The "fire function"
	for(y=1; y < (buffer_height); y++)
		for(x=0; x < buffer_width; x++)
		{
			// Get average color
			buffer[y-1][x] = (buffer[y][x] + buffer[y][x-1] + buffer[y][x+1] + buffer[y+1][x])/4;
			
			// Make colors darker
			if(buffer[y-1][x]) buffer[y-1][x]--;
			
			// This is a hack, since iit changes the layout of the palette we get junk data,
			// this will be fixed before the first release
			if(buffer[y-1][x] < 10) buffer[y-1][x] = 10;
			
		}

	
	// Flip/mirror the fire
	for(i=0; i < ((buffer_width*buffer_height)/2); i++)
	{
		((u8*)buffer)[i] = ((u8*)buffer)[(buffer_width*buffer_height)-i];
	}
}
