/****************************************
*                                       *
*            GSLIB by Hiryu             *
*                                       *
* gsDriver module:                      *
* Provides functions for initialisation *
* and configuration of the PS2's GS,    *
* aswell as support for GS management   *
* and multiple frame-buffers.           *
*                                       *
****************************************/

#include <tamtypes.h>
#include <kernel.h>
#include "hw.h"
#include "gsDefs.h"
#include "gsDriver.h"

void InitGraph(int interlace, int mode);

gsDriver::gsDriver()
{
	// Default to 2 buffers of 320x240x32bit with a 32bit ZBuffer
	setDisplayMode(320, 240, 85, 42, 
		GS_PSMCT32, 2, GS_TV_AUTO, GS_TV_NONINTERLACE,
		GS_ENABLE, GS_PSMZ32);
}

gsDriver::~gsDriver()
{
}

void gsDriver::setDisplayMode(unsigned int width, unsigned int height,
						   unsigned int xpos, unsigned int ypos,
						   unsigned int psm, unsigned int num_bufs,
						   unsigned int TVmode, unsigned int TVinterlace,
						   unsigned int zbuffer, unsigned int zpsm)
{

	// should put some boundary condition checking in here
	if (num_bufs < 1)
		num_bufs = 1; // we also need at least 1 frame buffer (should we set a max ?)

	m_FrameWidth = width & 0xFFC0; // must be a multiple of 64
	m_FrameHeight = height;
	m_FrameXpos = xpos;
	m_FrameYpos = ypos;
	m_FramePSM = psm;
	m_NumFrameBuffers = num_bufs;
	m_ZBuffer = zbuffer & 0x01;
	m_ZBufferPSM = zpsm;

	m_CurrentDisplayBuffer = 0;

	if (m_NumFrameBuffers > 1)
		m_CurrentDrawBuffer = 1;
	else
		m_CurrentDrawBuffer = 0;

	// Current Display buffer, and current draw buffer are not free
	// So set how many other buffers are free
	m_FreeBuffersAvailable = m_NumFrameBuffers-2;

	// Calculate size taken up by frame buffers;
	m_FrameSize = m_FrameWidth * m_FrameHeight * getBytesPerPixel(m_FramePSM);

	if ((m_FrameSize & 0x7FFF) != 0) // if framesize isnt a multiple of 8KB then round it up
	{
		m_FrameSize = (m_FrameSize & 0xFFFF8000) + 0x8000;
	}


	// Calculate the base address for the Z Buffer
	m_ZBufferBase = m_FrameSize * m_NumFrameBuffers;

	// If applicable set the zbuffer base and size
	if (m_ZBuffer)
	{
		m_ZBufferSize = m_FrameWidth * m_FrameHeight * getBytesPerPixel(m_ZBufferPSM);

		if ((m_ZBufferSize & 0x7FFF) != 0) // if zbuffer size isnt a multiple of 8KB then round it up
		{
			m_ZBufferSize = (m_ZBufferSize & 0xFFFF8000) + 0x8000;
		}

	}
	else
	{
		m_ZBufferSize = 0;
	}

	// Set the Texture Buffer Base
	m_TextureBufferBase = m_ZBufferBase + m_ZBufferSize;

	/* START OF GS HARDWARE SETUP */

	// Reset the GS
	GS_CSR = 0x00000200;
	__asm__ ("sync.p");
	GS_CSR = 0x00000000;

	// put IMR thru bios (GsPutIMR)
	__asm__("	ori $4, $0, 0xff00");
	__asm__("	addiu $3, $0, 0x0071");
	__asm__("	syscall");
	__asm__("	nop");

	InitGraph(TVinterlace, TVmode);

	// Now actually setup GS (from GS_SetDispMode)
	__asm__(" di ");

	// GS_PMODE = 0xFF60;	// Read Circuit disabled
	GS_PMODE = 0xFF61; // Read Circuit Enabled

	GS_DISPLAY1 =  GS_SET_DISPLAY(m_FrameWidth, m_FrameHeight, m_FrameXpos, m_FrameYpos);

	GS_BGCOLOUR = 0x000000; // Set the BGCOLOUR to black (overridable by user)

	__asm__(" ei ");


	// Set Current Display and Draw Buffers
	setDisplayBuffer(m_CurrentDisplayBuffer);
	setDrawBuffer(m_CurrentDrawBuffer);

	drawPipe.setZBuffer(m_ZBufferBase, m_ZBufferPSM, m_ZBuffer);
	drawPipe.setZTestEnable(GS_DISABLE);

	drawPipe.setOrigin(1024,1024);

	drawPipe.setPrModeCont(GS_ENABLE);

	drawPipe.setDither(GS_DISABLE);
	drawPipe.setColClamp(GS_ENABLE);

	drawPipe.setScissorRect(0,0,m_FrameWidth,m_FrameHeight);

	drawPipe.setZTestEnable(m_ZBuffer);
	drawPipe.Flush();

}

void gsDriver::setDisplayPosition(unsigned int xpos, unsigned int ypos)
{
	m_FrameXpos = xpos;
	m_FrameYpos = ypos;

	GS_DISPLAY1 =  GS_SET_DISPLAY(m_FrameWidth, m_FrameHeight, m_FrameXpos, m_FrameYpos);
}


void gsDriver::clearScreen(void)
{
	drawPipe.setZTestEnable(GS_DISABLE);
	drawPipe.RectFlat(0,0,m_FrameWidth,m_FrameHeight,0,GS_SET_RGBA(0x00,0x00,0x00,0x80));
	drawPipe.setZTestEnable(m_ZBuffer);
	drawPipe.Flush();

	swapBuffers();
}



// Why doesn't this work if part of a C++ class ?
// (yet it does work inside a C++ module)
void InitGraph(int interlace, int mode)
{
	__asm__(" # sceSetGSCrt ");
	__asm__("	ori $6, $0, 0           # field mode");
	__asm__("	addiu $3, $0, 2");
	__asm__("	syscall");
	__asm__("	nop");

	return;
}

// Get the base (relative to base of GS Mem) of the specified frame-buffer
unsigned int gsDriver::getFrameBufferBase(unsigned int fb_num)
{
	if (fb_num > (m_NumFrameBuffers-1))
		fb_num = (m_NumFrameBuffers-1);

	return (m_FrameSize * fb_num);
}

// Get the base (relative to base of GS Mem) of the Texture Buffer
unsigned int gsDriver::getTextureBufferBase(void)
{
	return m_TextureBufferBase;
}

unsigned int gsDriver::getCurrentDisplayBuffer(void)
{
	return m_CurrentDisplayBuffer;
}

unsigned int gsDriver::getCurrentDrawBuffer(void)
{
	return m_CurrentDrawBuffer;
}


void gsDriver::swapBuffers(void)
{
	// Indicate that we have finished drawing
	DrawBufferComplete();

	// Display the next frame
	DisplayNextFrame();

	// Since we're using normal double-buffers, and not swapping on interrupt,
	// then there will always be a next draw buffer available
	setNextDrawBuffer();
}

bool gsDriver::isDrawBufferAvailable()
{
	if (m_FreeBuffersAvailable)
		return 1;
	else
		return 0;
}

bool gsDriver::isDisplayBufferAvailable()
{
	if (m_CompleteBuffersAvailable)
		return 1;
	else
		return 0;
}

void gsDriver::setNextDrawBuffer(void)
{
	// If there is a free buffer available for drawing to ...
	if (m_FreeBuffersAvailable)		
	{
		m_CurrentDrawBuffer++;		// Move onto next buffer

		if (m_CurrentDrawBuffer > (m_NumFrameBuffers-1))
			m_CurrentDrawBuffer=0;

		setDrawBuffer(m_CurrentDrawBuffer); // Set Draw Environment for new buffer

		m_FreeBuffersAvailable--;	// Decrement number of free buffers available
	}
}

void gsDriver::DrawBufferComplete(void)
{
	// No need to do this here, since the next one isn't actually being used yet,
	// and this will be done by setNextDrawBuffer()
	//if (m_FreeBuffersAvailable)
	//	m_FreeBuffersAvailable --;	// Indicate that there is one less buffer free

	if (m_CompleteBuffersAvailable < (m_NumFrameBuffers-1))		// should this be -1 or -2 ?
		m_CompleteBuffersAvailable ++;// Indicate that there is one more buffer complete

	// should this automatically set next draw buffer ?
	// (probably not since this makes it harder for user app to decide whether it is okay to draw next frame)

}

void gsDriver::DisplayNextFrame(void)
{
	// If there is a complete buffer available for display ...
	if (m_CompleteBuffersAvailable)
	{
		m_CurrentDisplayBuffer++;		// Move onto next buffer

		if (m_CurrentDisplayBuffer > (m_NumFrameBuffers-1))
			m_CurrentDisplayBuffer=0;

		setDisplayBuffer(m_CurrentDisplayBuffer);	// Set Display to new buffer

		m_CompleteBuffersAvailable--;	// Decrement number of complete buffers available
		m_FreeBuffersAvailable++;	// Finished with previous buffer, so mark it as free
	}
}


void gsDriver::setDisplayBuffer(unsigned int buf_num)
{
	GS_DISPFB1 = GS_SET_DISPFB(getFrameBufferBase(buf_num), m_FrameWidth, m_FramePSM, 0, 0);
}

void gsDriver::setDrawBuffer(unsigned int buf_num)
{
	drawPipe.Flush();
	drawPipe.setDrawFrame(getFrameBufferBase(buf_num), m_FrameWidth, m_FramePSM, 0);
	drawPipe.Flush();
}

// Returns the ID of the VSync Callback
unsigned int gsDriver::AddVSyncCallback(void (*func_ptr)())
{
	unsigned int AddCallbackID;

	asm __volatile__ ("	di ");

	asm __volatile__ ("	# 'func_ptr' param will have been passed in $4, so move it to $5 (needed for syscall)");

	asm __volatile__ ("	addu  $5, $0, %0" :: "g" (func_ptr));
	asm __volatile__ ("	addiu $4, $0, 2		# 2 = vsync_start");	

	asm __volatile__ ("	addiu $6, $0, 0");
	asm __volatile__ ("	addiu $3, $0, 16	# AddIntcHandler");

	asm __volatile__ ("	syscall				# Returns assigned ID in $2");
	asm __volatile__ ("	nop");

	asm __volatile__ ("	addu  %0, $0, $2" : "=r" (AddCallbackID));
		
	asm __volatile__ ("	addiu $4, $0, 2	 ");
	asm __volatile__ ("	addiu $3, $0, 20");

	asm __volatile__ ("	syscall");
	asm __volatile__ ("	nop");

	asm __volatile__ ("	ei");

//		la $4, AddCallbackID	# Store ID in var
//		sw $2, 0($4)


	// Enable VSync callbacks if not already enabled
	EnableVSyncCallbacks();

	return AddCallbackID;
}


void gsDriver::RemoveVSyncCallback(unsigned int RemoveID)
{
	asm __volatile__ ("	di ");
	asm __volatile__ ("	addu  $5, $0, %0	# RemoveID will have already been passed into this func in $4" :: "g" (RemoveID));
	asm __volatile__ ("	addiu $4, $0, 2		# 2 = vsync_start");
	asm __volatile__ ("	addiu $3, $0, 17	# RemoveIntcHandler");
	asm __volatile__ ("	syscall");
	asm __volatile__ ("	nop");
	asm __volatile__ ("	ei");
}

void gsDriver::EnableVSyncCallbacks(void)
{

	asm __volatile__ ("	di");
	asm __volatile__ ("	addiu $4, $0, 2	 ");
	asm __volatile__ ("	addiu $3, $0, 20");

	asm __volatile__ ("	syscall");
	asm __volatile__ ("	nop");

	asm __volatile__ ("	ei");
}

void gsDriver::DisableVSyncCallbacks(void)
{
	asm __volatile__ ("	di");

	asm __volatile__ ("	addiu $4, $0, 2	");
	asm __volatile__ ("	addiu $3, $0, 21");

	asm __volatile__ ("	syscall");
	asm __volatile__ ("	nop");


	asm __volatile__ ("	ei");
}
