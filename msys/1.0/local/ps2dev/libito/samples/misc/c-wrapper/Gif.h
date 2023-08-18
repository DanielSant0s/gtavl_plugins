#ifndef _ITO_CGIF_H
#define _ITO_CGIF_H

#include <tamtypes.h>

/* It would be possible to write the GsDefs.h header to make it work in both C++ and C,
   but I will just copy paste the defines here :) */

/** Video Setup. */
#define	VMODE_NTSC					2
#define	VMODE_PAL					3
#define VMODE_AUTO					(((*((u8*)0x1FC7FF52))=='E')+2)
#define	FRAME						1
#define FIELD						0
#define ENABLE						1
#define DISABLE						0
#define RGBA16						2
#define NOZBUF						32
#define RGB(r,g,b)					(((b) << 16) | ((g) << 8) | (r))

void itoGsReset();
u32 itoGsGetVideoMode();
void itoGifSetupEnv(u32 psm, u32 width, u32 height, u32 bufferCount, u32 vmode, u32 ffmode, u32 interlace, u32 x, u32 y, u32 zpsm);
void itoGifSprite(u64 color, s32 x1, s32 y1, s32 x2, s32 y2, u32 z);
void itoGifClearScreen();
void itoGifSetNextDrawbuffer();
void itoGifSend();


#endif
