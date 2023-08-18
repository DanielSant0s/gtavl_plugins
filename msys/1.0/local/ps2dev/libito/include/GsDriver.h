#ifndef _ITO_GSDRIVER_H
#define _ITO_GSDRIVER_H

#include <ItoTypes.h>
#include <GsDefs.h>
#include <GifPacket.h>
#include <TextureManager.h>

namespace Ito
{
	class GsDriver;
}
/** Graphics Synthesizer Setup Driver. 
	
	\par About
	GsDriver is responsible for initializing the GS, setting up a rendering enviroment. The reason for this being
	a seperate class and not part of GifPipe, is because it can be the case that you only use GsDriver to init the
	GS and then only use PATH1/2 for rendering and never touch GifPipe.

	\par Usage
	Please see samples in samples/gif directory for usage.

*/
class Ito::GsDriver
{	
	private:
		GifPacket *m_GifPacket;
	protected:
		volatile u16 m_UsedFramebuffers;
		u32	m_FramebufferSize;
		u32	m_FramebufferCount; // number of framebuffers
		u16	m_ActiveFramebuffer;
		u16	m_VisibleFramebuffer;
		u32	m_Circuit;
		u32	m_Context;
	public:
		u32	m_FramebufferPSM;
		u32	m_FramebufferZPSM;
		u32	m_FramebufferWidth;
		u32	m_FramebufferHeight;
		u64	m_PrimOffsetXY;
		u16	m_FieldsPrFrame; // fields pr. frame
		u64 m_RegisterZBUF;
	public:
		/** Constructor. */
		GsDriver();
		/** Destructor. */
		~GsDriver();
		/** Initializes the GS
			
			This function setups the framebuffers and optional zbuffer. It is also possible to set which context and output circuit
			should be used, but unless needed for a very special case, its recommended not to set these, but just use the default settings.
			

			\par Resolutions
			There are a couple of more or less standard resolutions recommend listed below, if you define your own resolution remember
			that each framebuffer must be on a 8192 byte align and the width must be a multiple of 64. If not the framebuffers are no 
			aligned, they will automaticly be aligned and may cause unwanted results.\n\n
			Most of the videomodes are avaliable as samples in the samples/videomode directory. In some case its possible to use frame/interlace
			mode with low resolution mode which normally requires frame/non-interlace to get a fake high resolution and then use circuit blend to 
			make it look even nicer, those modes aren't documented here, but are shown in some of the video mode samples (640x2xx-interlace, 512x2nn-interlace). Resolutions defined as 
			Width x Height x PSM ffmode/interlace. \n \n
			\b NTSC \n
			640 x 448 x RGBA32 Field / Interlace\n
			640 x 448 x RGB24  Field / Interlace\n
			640 x 448 x RGBA16 Field / Interlace\n
			640 x 224 x RGBA32 Frame / Non-interlace\n
			640 x 224 x RGBA16 Frame / Non-interlace\n
			512 x 448 x RGBA32 Field / Interlace\n
			512 x 448 x RGBA24 Field / Interlace\n
			512 x 448 x RGBA16 Field / Interlace\n
			512 x 384 x RGBA32 Field / Interlace\n
			512 x 384 x RGBA16 Field / Interlace\n
			512 x 224 x RGBA32 Frame / Non-interlace\n
			512 x 224 x RGBA16 Frame / Non-interlace\n
			320 x 224 x RGBA32 Frame / Non-interlace\n
			\n \b PAL \n
			640 x 512 x RGBA32 Field / Interlace\n
			640 x 512 x RGB24  Field / Interlace\n
			640 x 512 x RGBA16 Field / Interlace\n
			640 x 256 x RGBA32 Frame / Non-interlace\n
			640 x 256 x RGBA16 Frame / Non-interlace\n
			512 x 512 x RGBA32 Field / Interlace\n
			512 x 512 x RGBA24 Field / Interlace\n
			512 x 512 x RGBA16 Field / Interlace\n
			512 x 384 x RGBA32 Field / Interlace\n
			512 x 384 x RGBA16 Field / Interlace\n
			512 x 256 x RGBA32 Frame / Non-interlace\n
			512 x 256 x RGBA16 Frame / Non-interlace\n
			320 x 256 x RGBA32 Frame / Non-interlace\n
			
			

			\param psm pixel storage mode. <tt>RGBA32, RGB24, RGBA16</tt>
			\param width framebuffer width
			\param height framebuffer height
			\param bufferCount number of framebuffers, most common value is 2
			\param vmode video mode. <tt>VMODE_AUTO, VMODE_NTSC, VMODE_PAL</tt>
			\param ffmode frame/field mode. <tt>FRAME, FIELD</tt>
			\param interlace enable/disable interlace. <tt>ENABLE, DISABLE</tt>
			\param x display offset x.
			\param y display offset y.
			\param zpsm (optional) zbuffer pixel storage mode <tt> ZPSM32, ZPSM24, ZPSM16, NO_ZBUF for no zbuffer (default)</tt>
			\return true on success otherwise false. */
		bool SetupEnv(u32 psm, u32 width, u32 height, u32 bufferCount, u32 vmode, u32 ffmode, u32 interlace, u32 x, u32 y, u32 zpsm = NOZBUF);
		
		/** Upload Texture */
		bool UploadTexture(Texture& texture, bool block = 0);
		/** SetNextFramebuffer. If there is no avaliable framebuffer, this function will block until there is one. 
			Since the buffer switch is done by sending a packet to GS, this function will also wait until the transfer
			has completed.
		*/
		void SetNextDrawbuffer();
		/** ShowNextFramebuffer. This will show the next fully rendered framebuffer, if the framebuffer is still rendering, it will
			do nothing.
		
			\return true if switched to next framebuffer, false if not.
		*/
		bool ShowNextFramebuffer();	
		/** Set a GS General Purpose Register. */
		void SetRegister(u64 address, u64 data, bool block = true);
		/** Setup Circuit Blend aka "De-interlace filter" aka "Antialias filter". */
		bool CircuitBlend(bool enable, s32 alpha);	
};

#endif
