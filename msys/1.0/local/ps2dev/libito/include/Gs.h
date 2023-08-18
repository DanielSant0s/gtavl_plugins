#ifndef _ITO_GS_H
#define _ITO_GS_H

#include <ItoTypes.h>

namespace Ito
{
	/** Graphics Related Helper Functions. */
	namespace Gs
	{
		void SetTextureBufferBase(u32 base);
		u32 GetTextureBufferBase();

		/** Bytes per pixel*/
		u32 GetPixelSize(s32 psm);
		/** Bytes per Z "pixel"/value */
		u32 GetZPixelSize(s32 mode);

		/** Get Texture Size in bytes. */
		u32 GetTextureSize(u32 psm, u32 width, u32 height);
		
		/** Set BGCOLOR register. Please refer to the GS Manual for further details. 
			\param color
		*/
		void SetBGCOLOR(u64 color);
		/** Set SMODE2 register. Please refer to the GS Manual for further details. 
			\param interlace, ffmode, vesa
		*/
		void SetSMODE2(u32 interlace, u32 ffmode, u32 vesa);
		/** Set PMODE register. Please refer to the GS Manual for further details. 
			\param rc1, rc2, mmod, amod, slbg, alpha
		*/
		void SetPMODE(u32 rc1, u32 rc2, u32 mmod, u32 amod, u32 slbg, u32 alpha);
		/** Set DISPFB1 register. Please refer to the GS Manual for further details. 
			\param fbp, fbw, psm, dbx, dby
		*/
		void SetDISPFB1(u32 fbp, u32 fbw, u32 psm, u32 dbx, u32 dby);
		/** Set DISPFB2 register. Please refer to the GS Manual for further details. 
			\param fbp, fbw, psm, dbx, dby
		*/
		void SetDISPFB2(u32 fbp, u32 fbw, u32 psm, u32 dbx, u32 dby);
		/** Set DISPLAY1 register. Please refer to the GS Manual for further details. 
			\param dx, dy, mag_x, mag_y, dw, dh
		*/
		void SetDISPLAY1(u32 dx, u32 dy, u32 mag_x, u32 mag_y, u32 dw, u32 dh);
		/** Set DISPLAY2 register. Please refer to the GS Manual for further details. 
			\param dx, dy, mag_x, mag_y, dw, dh
		*/
		void SetDISPLAY2(u32 dx, u32 dy, u32 mag_x, u32 mag_y, u32 dw, u32 dh);
		/** Get BGCOLOR register. Please refer to the GS Manual for further details. */
		u64 GetBGCOLOR();
		/** Get SMODE2 register. Please refer to the GS Manual for further details. */
		u64 GetSMODE2();
		/** Get PMODE register. Please refer to the GS Manual for further details. */
		u64 GetPMODE();
		/** Get DISPFB1 register. Please refer to the GS Manual for further details. */
		u64 GetDISPFB1();
		/** Get DISPFB2 register. Please refer to the GS Manual for further details. */
		u64 GetDISPFB2();
		/** Get DISPLAY1 register. Please refer to the GS Manual for further details. */
		u64 GetDISPLAY1();
		/** Get DISPLAY2 register. Please refer to the GS Manual for further details. */
		u64 GetDISPLAY2();

		void VSyncWait();

		void Reset();

		u32 GetVideoMode();

		/** TransferWait for Gif (PATH3) */
		u32 GifTransferWait(u32 mode = 0);

		/** Print GS Control Registers. */
		void PrintControlRegisters();
	}
}

#endif
