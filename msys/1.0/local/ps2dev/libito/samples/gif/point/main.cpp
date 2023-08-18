#include <Ito.h>
#include <stdlib.h> // for rand()
#include <stdio.h>

using namespace Ito;

// The famous star field :)

class Star
{
	private:
		s32 x;
		s32 y;
		s32 z;
		u64 color;
		static const s32 ZMax = 100;
		static const s32 Speed = 2;
	public:
		Star() 
		{
			x = 0;
			y = 0;
			z = Speed+(rand() % (ZMax-Speed));
			color = 0;
		}
		~Star() {}

		void Draw(GifPipe& gif, s32 width, s32 height)
		{
			s32 draw_x = (width/2)+(x/z);
			s32 draw_y = (height/2)+(y/z);	

			if((z <= Speed) || (draw_x < 1) || (draw_y < 1) || (draw_x > width-1) || (draw_y > height-1))
			{
				x = (rand()%(width*2*Speed))-(width*Speed);
				y = (rand()%(height*2*Speed))-(height*Speed);
				z = ZMax;
				color = RGB(32,32,32);
			}
			else
			{	
				// "Fade" effect
				if(z < (ZMax/2)) color = RGB(64, 64, 64);
				if(z < (ZMax/4)) color = RGB(128,128, 128);
				if(z < (ZMax/8)) color = RGB(192, 192, 192);
				if(z < (ZMax/16)) color = RGB(255, 255, 255);
				
				gif.Sprite(color, draw_x-1, draw_y-1, draw_x+1, draw_y+1);
				
				z -= Speed;	
		}	
	}


};

const u32 SIZE = 1000;

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

	Star stars[SIZE];

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		Gif.ClearScreen();
		
		for(u32 i=0; i < SIZE; i++)
			stars[i].Draw(Gif, ScreenWidth, ScreenHeight);
			
		Gif.Send();		
	}

	return 0;
}
