#include <tamtypes.h>
#include <kernel.h>
#include <fileio.h>
#include <malloc.h>
#include <stdio.h>

#include "gsDefs.h"
#include "gsDriver.h"
#include "gsPipe.h"


int main()
{
	int xpos=20;
	int ypos=20;
	int xdir=1;
	int ydir=1;

	int tex_width, tex_height;	// Size of IIF Texture

	IIF* tex_file;


	// Create a gsDriver with default settings
	// (320x240 32bit, double-buffer, zbuffer allocated, alpha enabled)
	gsDriver myGsDriver;	

	myGsDriver.drawPipe.setAlphaEnable(GS_ENABLE);

	// Won't bother creating our own gsPipe, since we can use the one created by our gsDriver
	// gsPipe myGsPipe;

	// Open the file
	int filehandle = fioOpen("host:texture.iif",O_RDONLY);

	// Get the size of the file
	int filesize = fioLseek( filehandle, 0, SEEK_END);

	// allocate space to load the file into EE mem
	tex_file = (IIF*)memalign(64,filesize);

	//seek back to the start of the file
	fioLseek(filehandle, 0, SEEK_SET);

	if (fioRead(filehandle,tex_file,filesize) <= 0)
	{
		printf("Could not load texture.iif\n");
		return -1;
	}

	fioClose(filehandle);

	tex_width=tex_file->width;
	tex_height=tex_file->height;

	printf("Texture = %dx%d\n",tex_width,tex_height);

	// Upload into the beginning of texture mem (with texture-buffer width set to 256)
	myGsDriver.drawPipe.TextureUpload(
		myGsDriver.getTextureBufferBase(), 256,
		0, 0, (int)tex_file->psm, (const unsigned char*)&(tex_file->pixel_data),
		tex_width, tex_height);

	// Set-up the Texture (can do this outside the draw loop in this example, since we are only using one texture)
	myGsDriver.drawPipe.TextureSet(
		myGsDriver.getTextureBufferBase(), 256,
		GS_TEX_SIZE_256, GS_TEX_SIZE_256, tex_file->psm, 
		0, 0, 0, 0);

	

	while (1)
	{
        // Clear the screen (with ZBuffer Disabled)
		myGsDriver.drawPipe.setZTestEnable(GS_DISABLE);
		myGsDriver.drawPipe.RectFlat(0,0,320,240,0,GS_SET_RGBA(0x00,0x00,0x00,0x80));
		myGsDriver.drawPipe.setZTestEnable(GS_ENABLE);

		// Draw a moving, solid, textured rectangle, with our texture on it (at z=1)
		myGsDriver.drawPipe.RectTexture(
			xpos, ypos, 0, 0,
			xpos+tex_width, ypos+tex_height, tex_width, tex_height,
			1, GS_SET_RGBA(0x80,0x80,0x80,0x80));

		// Draw a static, transparent-green rectangle in front of it (z=2)
		myGsDriver.drawPipe.RectFlat(120,60,120+80,60+60,2,GS_SET_RGBA(0,0xFF,0,0x40));

		// Flush the pipe, to draw the prims
		myGsDriver.drawPipe.Flush();


		// Wait for VSync and then swap buffers
		myGsDriver.WaitForVSync();

		myGsDriver.swapBuffers();

		// move the green rectangle
		xpos+=xdir;
		ypos+=ydir;

		// change dir if it hits the limits
		if (xpos>((320-tex_width)-1)) xdir=-1;
		if (xpos<1) xdir=1;

		if (ypos>((240-tex_height)-1)) ydir=-1;
		if (ypos<1) ydir=1;

	}
}
