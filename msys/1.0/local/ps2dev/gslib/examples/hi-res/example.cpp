#include <tamtypes.h>
#include <kernel.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"

int main()
{
	int xpos=30;
	int ypos=30;
	int xdir=1;
	int ydir=1;

	// Create a gsDriver with custom settings
	// (640x480 32bit, double-buffer, zbuffer allocated, alpha enabled)
	gsDriver myGsDriver;
	
	myGsDriver.setDisplayMode(640, 480, 170, 80, 
		GS_PSMCT32, 2, GS_TV_AUTO, GS_TV_INTERLACE,
		GS_ENABLE, GS_PSMZ32);	

	myGsDriver.drawPipe.setAlphaEnable(GS_ENABLE);
	// Won't bother creating our own gsPipe, since we can use the one created by our gsDriver
	// gsPipe myGsPipe;




	while (1)
	{
        // Clear the screen (with ZBuffer Disabled)
		myGsDriver.drawPipe.setZTestEnable(GS_DISABLE);
		myGsDriver.drawPipe.RectFlat(0,0,640,480,0,GS_SET_RGBA(0,0,0,0x80));
		myGsDriver.drawPipe.setZTestEnable(GS_ENABLE);
	
		// Draw some shapes

		// This is at the back (z=1), so will be behind the moving object
		myGsDriver.drawPipe.RectFlat(80,60,80+120,60+80,1,GS_SET_RGBA(0xFF,0,0,0x80));

		// This is also at the back (z=1)
		myGsDriver.drawPipe.TriangleGouraud(
			400, 300, 1, GS_SET_RGBA(0xFF,0x00,0x00,0x80),
			550, 320, 1, GS_SET_RGBA(0x00,0xFF,0x00,0x80),
			380, 460, 1, GS_SET_RGBA(0x00,0x00,0xFF,0x80));

		// This is near the back too (z=2)
		myGsDriver.drawPipe.TriangleLine(
			20,  240, 2, GS_SET_RGBA(0xFF,0xFF,0x00,0x80),
			220, 240, 2, GS_SET_RGBA(0xFF,0xFF,0x00,0x80),
			120, 390, 2, GS_SET_RGBA(0xFF,0xFF,0x00,0x80));

		// This moving transparent (alpha=0x40) Rectangle is in front of most of the other objects (z=3)
		myGsDriver.drawPipe.RectFlat(
			xpos, ypos,
			xpos+160, ypos+120,
			3, GS_SET_RGBA(0,0xFF,0,0x40));

		// but behind this solid gouraud 4-sided poly (z=4)
		myGsDriver.drawPipe.TriStripGouraud(
			400, 50,  4, GS_SET_RGBA(0xFF,0x00,0x00,0x80),
			550, 50,  4, GS_SET_RGBA(0xFF,0x00,0xFF,0x80),
			450, 200, 4, GS_SET_RGBA(0xFF,0x00,0xFF,0x80),
			600, 200, 4, GS_SET_RGBA(0x00,0x00,0xFF,0x80));


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
