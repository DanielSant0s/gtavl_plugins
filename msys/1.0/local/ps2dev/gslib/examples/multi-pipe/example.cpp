#include <tamtypes.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"

int main()
{
	// Create a gsDriver with default settings
	// (320x240 32bit, double-buffer, zbuffer allocated, alpha enabled)
	gsDriver myGsDriver;	

	// Lets try creating our own gsPipe, and using both at the same time
	// we must ReInit() each pipe when switching between them

	// Note: creating a new gsPipe will by default, disable ztest on the GS hardware
	// be careful to only re-enable ztest if the zbuffer is disabled.

	// (when using the gsDriver's gsPipe it will know whether the zbuffer is enabled or not,
	// and if it is disabled, it will not allow ztest to be enabled.

	gsPipe myGsPipe;


	// in this example we want to disable ztest anyway

	myGsDriver.drawPipe.setZTestEnable(GS_DISABLE);
	myGsPipe.setZTestEnable(GS_DISABLE);

	while (1)
	{
        // Clear the screen - using our gsPipe
		myGsDriver.drawPipe.RectFlat(0,0,320,240,0,GS_SET_RGBA(0,0,0,0x80));
		myGsDriver.drawPipe.Flush();
		
		// Draw a solid-red rectangle - using the gsDriver's gsPipe
		myGsDriver.drawPipe.RectFlat(80,60,200,140,0,GS_SET_RGBA(0xFF,0,0,0x80));

		// Draw a solid green rectangle overlapping it - using our gsPipe
		myGsPipe.RectFlat(40,100,120,180,1,GS_SET_RGBA(0,0xFF,0,0x80));

		// flush our gsPipe first so that the green rectangle actually gets drawn first
		myGsPipe.Flush();


		// now flush the gsDriver's gsPipe,
		// so that the red rectangle gets drawn over the green one,
		// rather than the other way around
		myGsDriver.drawPipe.Flush();


		// Wait for VSync and then swap buffers
		myGsDriver.WaitForVSync();

		myGsDriver.swapBuffers();
	}
}
