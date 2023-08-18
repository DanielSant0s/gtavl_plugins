#include <tamtypes.h>
#include <kernel.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"

extern int* _args;

int main(int argc, char **argv)
{
	int xpos=20;
	int ypos=20;
	int xdir=1;
	int ydir=1;

	// Create a gsDriver with default settings
	// (320x240 32bit, double-buffer, zbuffer allocated, alpha enabled)
	gsDriver myGsDriver;

	myGsDriver.drawPipe.setAlphaEnable(GS_ENABLE);

	// Won't bother creating our own gsPipe, since we can use the one created by our gsDriver
	// gsPipe myGsPipe;

	while (1)
	{
        // Clear the screen (with ZBuffer Disabled)
		myGsDriver.drawPipe.setZTestEnable(GS_DISABLE);
		myGsDriver.drawPipe.RectFlat(0,0,320,240,0,GS_SET_RGBA(0,0,0,0x80));
		myGsDriver.drawPipe.setZTestEnable(GS_ENABLE);
		
		// Draw a solid-red rectangle (at z=1)
		myGsDriver.drawPipe.RectFlat(80,60,200,140,1,GS_SET_RGBA(0xFF,0,0,0x80));

		// Draw a moving transparent-green rectangle in front of it (z=2)
		myGsDriver.drawPipe.RectFlat(xpos,ypos,xpos+80,ypos+60,2,GS_SET_RGBA(0,0xFF,0,0x40));

		// Flush the pipe, to draw the prims
		myGsDriver.drawPipe.Flush();


		// Indicate that we have finished drawing
		myGsDriver.DrawBufferComplete();

		// Wait for VSync and then swap buffers
		myGsDriver.WaitForVSync();

		// Display the next frame
		myGsDriver.DisplayNextFrame();

		// Since we're using normal double-buffers, and not swapping on interrupt,
		// then there will always be a next draw buffer available
		myGsDriver.setNextDrawBuffer();


		xpos+=xdir;
		ypos+=ydir;

		// change dir if it hits the limits
		if (xpos>((320-80)-1)) xdir=-1;
		if (xpos<1) xdir=1;

		if (ypos>((240-60)-1)) ydir=-1;
		if (ypos<1) ydir=1;
	}

	return 0;
}
