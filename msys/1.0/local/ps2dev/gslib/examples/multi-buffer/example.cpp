#include <tamtypes.h>
#include <kernel.h>
#include <stdio.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"

//global vsync counter
volatile int vsync_num;
int frame_num;

// Create a global pointer to a gsDriver, so it can be called from the vsync callback
gsDriver* pMyGsDriver;	

void render_main(void);

// declare callback function
void vsync_func(void)
{
	// We had to put our gsDriver in global scope, so that it could be accessed from this callback

	// Display the next frame if there is one ready

	// If there is a drawn frame available for display this will display the new one,
	// and mark the old displayed frame as free for drawing
	// otherwise it will do nothing

	pMyGsDriver->DisplayNextFrame();

	//increment our vsync counter
	vsync_num++;

	asm __volatile__ ("ei");
}

int main()
{
	// Setup Display Mode
	// (320x240 32bit, 5 display buffers, zbuffer allocated, alpha enabled)
	pMyGsDriver = new gsDriver;

	printf("\n2 out of 4 frames will take upto 2 frame-times to draw !\n\n");
	printf("First lets try it with a conventional double-buffer ...\n");

	pMyGsDriver->setDisplayMode(320, 240, 85, 42,
		GS_PSMCT32, 2, GS_TV_AUTO, GS_TV_NONINTERLACE,
		GS_ENABLE, GS_PSMZ32);

	pMyGsDriver->drawPipe.setAlphaEnable(GS_ENABLE);

	render_main();
	
	printf("We have managed to rendered %d out of %d frames.\n\n", frame_num, vsync_num);

	printf("Now lets try it with 4 frame buffers ...\n");

	pMyGsDriver->setDisplayMode(320, 240, 85, 42, 
		GS_PSMCT32, 4, GS_TV_AUTO, GS_TV_NONINTERLACE,
		GS_ENABLE, GS_PSMZ32);

	pMyGsDriver->drawPipe.setAlphaEnable(GS_ENABLE);

	// We can draw 4 frames before we have to display the next one
	// This allows us to render ahead on simpler frames, so that we can draw
	// occasional frames that take more than one frame-time to draw,
	// without dropping any frames

	// The best way to do this is to have the display update handled in a vsync ISR
	// so that if a drawn frame is available for display it will automatically display it

	// Our main-line can then render as quick/slow as it wants, regardless of vsync timing,
	// aslong as there is a free frame available for drawing.

	render_main();

	printf("We have managed to rendered %d out of %d frames,\n", frame_num, vsync_num);
	printf("despite 2 out of every 4 frames taking upto 2 frame-times to render !\n");

	printf("\n(frame numbers may be out by one due to time taken\n"
		"to enable/disable vsync callback function\n");
	
	return 0;
}


void render_main(void)
{
	unsigned int callbackID;
	int xpos=30;
	int xdir=1;
	
	frame_num = 0;

	// lets add our vsync ISR
	callbackID = pMyGsDriver->AddVSyncCallback(&vsync_func);

	vsync_num=0;

    // Do rendering for 500 vsyncs
	while (vsync_num<500)
	{
        // Clear the screen (with ZBuffer Disabled)
		pMyGsDriver->drawPipe.setZTestEnable(GS_DISABLE);
		pMyGsDriver->drawPipe.RectFlat(0,0,320,240,0,GS_SET_RGBA(0,0,0,0x80));
		pMyGsDriver->drawPipe.setZTestEnable(GS_ENABLE);
		
		// Draw a solid-red rectangle (at z=1)
		pMyGsDriver->drawPipe.RectFlat(80,60,200,140,1,GS_SET_RGBA(0xFF,0,0,0x80));

		// Draw a moving transparent-green rectangle in front of it (z=2)
		pMyGsDriver->drawPipe.RectFlat(xpos,120,xpos+80,180,2,GS_SET_RGBA(0,0xFF,0,0x40));

		// Flush the pipe, to draw the prims
		pMyGsDriver->drawPipe.Flush();

		frame_num++;

		// 2 out of every 4 frames, take upto 2 frame-times to render (as an example)
		if ((frame_num & 0x03) >= 2)
		{
			pMyGsDriver->WaitForVSync();
			pMyGsDriver->WaitForVSync();
		}

		// Indicate that we have finished drawing
		pMyGsDriver->DrawBufferComplete();


		// if no new buffer is available for drawing, then wait until one is
		while(!(pMyGsDriver->isDrawBufferAvailable()));

		// once buffer is available then set the next display buffer and continue drawing
		pMyGsDriver->setNextDrawBuffer();

		// move the green rectangle
		xpos+=xdir;;

		// change dir if it hits the limits
		if (xpos>220) xdir=-1;
		if (xpos<30) xdir=1;

	}

	pMyGsDriver->RemoveVSyncCallback(callbackID);

	// removed last callback so disable handler
	pMyGsDriver->DisableVSyncCallbacks();
}




