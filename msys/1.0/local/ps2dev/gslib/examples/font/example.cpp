#include <tamtypes.h>
#include <kernel.h>
#include <fileio.h>
#include <malloc.h>
#include <stdio.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"
#include "gsFont.h"


int main()
{
	int xpos=20;
	int ypos=20;
	int xdir=1;
	int ydir=1;


	gsFontTex* fontTex;


	// Create a gsDriver with default settings
	// (320x240 32bit, double-buffer, zbuffer allocated, alpha enabled)
	gsDriver myGsDriver;

	// Change to hi-res so can fit more of our 16x16 font on screen
	myGsDriver.setDisplayMode(640, 480, 170, 80, 
		GS_PSMCT32, 2, GS_TV_AUTO, GS_TV_INTERLACE,
		GS_ENABLE, GS_PSMZ32);	

	gsFont myFont;

	// Won't bother creating our own gsPipe, since we can use the one created by our gsDriver
	// gsPipe myGsPipe;

	// Assign a gsPipe to the font handler
	myFont.assignPipe(&myGsDriver.drawPipe);

	// Enable alpha blending
	myGsDriver.drawPipe.setAlphaEnable(GS_ENABLE);


	// Open the file
	int filehandle = fioOpen("host:arial.fnt",O_RDONLY);

	// Get the size of the file
	int filesize = fioLseek( filehandle, 0, SEEK_END);

	// allocate space to load the file into EE mem
	fontTex = (gsFontTex*)memalign(64,filesize);

	//seek back to the start of the file
	fioLseek(filehandle, 0, SEEK_SET);

	if (fioRead(filehandle,fontTex,filesize) <= 0)
	{
		printf("Could not load texfont.fnt\n");
		while(1);
	}

	fioClose(filehandle);

	// Upload into the beginning of texture mem (with texture-buffer width set to 256)
	myFont.uploadFont(fontTex, myGsDriver.getTextureBufferBase(), 
		fontTex->TexWidth, // Use the fontTex width as texbuffer width (can use diff width)
		0, 0 );

	while (1)
	{
        // Clear the screen (with ZBuffer Disabled)
		myGsDriver.drawPipe.setZTestEnable(GS_DISABLE);
		myGsDriver.drawPipe.RectFlat(0,0,640,480,0,GS_SET_RGBA(0x00,0x00,0x00,0x80));
		myGsDriver.drawPipe.setZTestEnable(GS_ENABLE);


		// WHY IS THIS FUBAR IF DRAWN OUT OF SEQUENCE ? (Z-Test should make it okay)

		// Draw a static, solid red rectangle in the background (z=1)
		myGsDriver.drawPipe.RectFlat(
			120, 60, 120+120, 60+80,
			1, GS_SET_RGBA(0xFF,0x00,0x00,0x7F));


		// Print some text behind the moving transparent rectangle, but in-front of the static solid one

		myFont.Print(20, 400, 40, 2, GS_SET_RGBA(0xFF,0xFF,0xFF,0x80), GSFONT_ALIGN_LEFT,
			"This is some left-aligned text on the first line,\n"
			"and this is some more text.\n"
			"Don't get too excited, this is just even more text to prove that line-wrap works properly.");

		myFont.Print(140, 500, 200, 2, GS_SET_RGBA(0x20,0x20,0xFF,0x80), GSFONT_ALIGN_CENTRE,
			"This is some blue centred text.\n"
			"You can do some nice \aunderlining\a if you want to!\n"
			"It's easy, just use \\a to turn it on or off");

		myFont.Print(20, 620, 360, 2, GS_SET_RGBA(0xFF,0x20,0x20,0x80), GSFONT_ALIGN_RIGHT,
			"This is some red right-aligned text.\n"
			"You can do some nice \bbold\b text too if you want!\n"
			"Just use \\b to turn bold text on or off");



		// Draw a moving, semi-transparent, rectangle (at z=3)
		myGsDriver.drawPipe.RectFlat(
			xpos, ypos, xpos+160, ypos+120,
			3, GS_SET_RGBA(0x00,0xFF,0x00,0x40));



		// Flush the pipe, to draw the prims
		myGsDriver.drawPipe.Flush();



		// Wait for VSync and then swap buffers
		myGsDriver.WaitForVSync();

		myGsDriver.swapBuffers();

		// move the green rectangle
		xpos+=xdir;
		ypos+=ydir;

		// change dir if it hits the limits
		if (xpos>((640-160)-1)) xdir=-1;
		if (xpos<1) xdir=1;

		if (ypos>((480-120)-1)) ydir=-1;
		if (ypos<1) ydir=1;

	}
}
