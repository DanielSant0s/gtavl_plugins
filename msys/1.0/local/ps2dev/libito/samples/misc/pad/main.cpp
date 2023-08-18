#include <Ito.h>
#include <stdio.h>

// You need a joypad inserted into joypad port 1 to make this example work, it also works in 2 player mode, although optional.

// Defines 
#define bgcolor1		RGBA(0x40, 0x40, 0xFF, 0)
#define bgcolor2		RGBA(0x80, 0x80, 0x80, 0)
#define batcolor		RGBA(0xFF, 0xFF, 0xFF, 0)
#define ballcolor		RGBA(0xFF ,0xFF, 0xFF, 0)
#define bordercolor		RGBA(0xFF ,0xFF, 0xFF, 0)

#define batwidth		16
#define batheight		44
#define ballsize		16
#define border_offset	8

using namespace Ito;

GifPipe	Gif;
Pad Pad1;
Pad Pad2;

u32 ScreenWidth  = 640;
u32 ScreenHeight = 224;

u32 cpu_mode = 0;
u32 winner = 0;

s32 bat1_pos;
s32 bat2_pos;

u32 ball_dir;
float  ball_angle;
float  ball_speed;
s32 ball_x;
s32 ball_y;


void Reset()
{
	bat1_pos	= bat2_pos = ScreenHeight/2;
	
	ball_angle	= 0.0f;
	ball_speed  = 1.0f;
	
	if(winner == 1)
		ball_dir = 0;
	else
		ball_dir = 1;

	ball_x		= ScreenWidth/2;
	ball_y		= ScreenHeight/2;
}

void ClearScreen()
{
	Gif.Triangle(bgcolor2,  0,ScreenHeight, 0,
					bgcolor1,  0, 0, 0,
					bgcolor1,  ScreenWidth, 0 , 0);

	Gif.Triangle(bgcolor1,  ScreenWidth, 0, 0,
					bgcolor2,  ScreenWidth, ScreenHeight, 0,
					bgcolor2,  0, ScreenHeight , 0);
		
}

void DrawBorders()
{
	Gif.Line(bordercolor, 30, border_offset, 0, bordercolor, ScreenWidth-30, border_offset, 0);
	Gif.Line(bordercolor, 30, ScreenHeight-border_offset, 0, bordercolor, ScreenWidth-30, ScreenHeight-border_offset, 0);
}

void DrawBat(s32 player, s32 y)
{
	u32 x;

	y -= (batheight/2);
	
	if(player == 0)
		x = 30;
	else
		x = ScreenWidth-30-batwidth;

	if(y < 0)
		y = 0;

	if( (y+batheight) > (s32)ScreenHeight)
		y = ScreenHeight-batheight;
	
	y += (batheight/2);

	Gif.Sprite(batcolor, x, y-(batheight/2), x+batwidth, y+(batheight/2), 0);

}

void DrawBall()
{
	Gif.Sprite(ballcolor, ball_x-(ballsize/2), ball_y-(ballsize/4), ball_x+(ballsize/2), ball_y+(ballsize/4), 0);
}

void ReadPads()
{
	bat1_pos -= (batheight/2);
	bat2_pos -= (batheight/2);

	// Read pad 1
	u16 buttons1 = Pad1.ReadButtons();

	if(buttons1 != 0xFFFF)
	{
		if(buttons1 & PAD_UP)		bat1_pos-=4;
		if(buttons1 & PAD_DOWN)		bat1_pos+=4;
	}

	if(bat1_pos < border_offset) bat1_pos = border_offset;
	if((bat1_pos+batheight) > (s32)(ScreenHeight-border_offset)) bat1_pos = ScreenHeight-batheight-border_offset;


	if(cpu_mode == 1)
	{
		// CPU mode

		if(ball_y < (bat2_pos+(batheight/2))) bat2_pos-=3;
		if(ball_y > (bat2_pos+(batheight/2))) bat2_pos+=3;
	}
	else
	{
		// Read pad 2
		u16 buttons2 = Pad2.ReadButtons();

		if(buttons2 != 0xFFFF)
		{
			if(buttons2 & PAD_UP)		bat2_pos-=4;
			if(buttons2 & PAD_DOWN)		bat2_pos+=4;
		}
	}	

		if(bat2_pos < border_offset) bat2_pos = border_offset;
		if((bat2_pos+batheight) > (s32)(ScreenHeight-border_offset)) bat2_pos = ScreenHeight-batheight-border_offset;


	
	bat1_pos += (batheight/2);
	bat2_pos += (batheight/2);

	DrawBat(0, bat1_pos);
	DrawBat(1, bat2_pos);
}

// The heart of this pong game
void MoveBall()
{
	s32 ball_offset = 0;
	
	ball_x -= ballsize/2;
	ball_y -= ballsize/2;

	// Check against border collision
	if( ball_y < border_offset )
		ball_angle = 0.0f - ball_angle;

	if((ball_y+(ballsize)) > (s32)(ScreenHeight-border_offset)) 
		ball_angle = 0.0f - ball_angle;

	// Move the ball
	if(ball_dir == 0)
		ball_x += 3*(s32)ball_speed;
	else
		ball_x -= 3*(s32)ball_speed;
	
	ball_y += (s32)(1.5f*ball_angle);

	// Some one just lost, ball out of bounds :)
	if( (ball_x < 0))
	{
		winner = 1;
		Reset();
	}
	
	if((ball_x+ballsize) > (s32)ScreenWidth)
	{
		winner = 0;
		Reset();
	}

	// Check for collision with bats
	ball_offset = 1000;
	
	// Bat 1 (left)
	if( (ball_x <= (30+batwidth)) && (ball_x >= 30) )
		ball_offset = (ball_y+(ballsize/2)) - (bat1_pos-(batheight/2));

	// Bat 2 (right)
	if( (ball_x >= (s32)(ScreenWidth-30-batwidth-ballsize)) && (ball_x <= (s32)(ScreenWidth-30-ballsize)) )
		ball_offset = (ball_y+(ballsize/2)) - (bat2_pos-(batheight/2));

	if(ball_offset != 1000)
	{
		// Ball was hit by bat (?)
		if( (ball_offset > 0) && (ball_offset < batheight))
		{	
			// Change ball direction
			ball_dir = 1 - ball_dir;
		
			// Ball hit top part of bat
			if(ball_offset > (batheight/2))
			{
				ball_angle += 1.2f;	
				
				if(ball_angle == 0.0f) ball_angle += 0.8f;
			}
			
			// Ball hit bottom part of bat
			if(ball_offset < (batheight/2))
			{
				ball_angle -= 1.2f;
				
				if(ball_angle == 0.0f) ball_angle -= 0.8f;	
			}
		}
		// Increase ball speed for each hit
		ball_speed += 0.2f;
	}

	ball_x += ballsize/2;
	ball_y += ballsize/2;

	DrawBall();
}

int main()
{
	u32 ScreenX = 155;
	u32 ScreenY = 70;

	if(Gs::GetVideoMode() == VMODE_PAL)  
	{
		ScreenHeight = 256;
		ScreenY = 70;
	}
	Gs::Reset();
	
	Gif.SetupEnv(RGBA32, ScreenWidth, ScreenHeight, 2, VMODE_AUTO, FRAME, ENABLE, ScreenX, ScreenY);

	Gif.PrimShading(PRIM_GOURAUD);

	Reset(); // Reset game

	Pad1.Open(0,0);
	
	printf("Waiting for pad 1\n");
	while(Pad1.GetState() != PAD_STATE_STABLE);
	printf("Pad 1 is ready\n");



	Pad2.Open(1,0);

	printf("Waiting for pad 2\n");

	if(Pad2.GetState() != PAD_STATE_STABLE)
	{
		cpu_mode = 1;
		printf("Pad 2 not connected, playing against CPU\n");
	}
	else
	{
		cpu_mode = 0;
		printf("Pad 2 is ready\n");
	}

	while(1)
	{
		Gif.SetNextDrawbuffer();	
		
		ClearScreen();
		
		DrawBorders();

		ReadPads();
			
		MoveBall();

		Gif.Send();
	}


	return 0;
}
