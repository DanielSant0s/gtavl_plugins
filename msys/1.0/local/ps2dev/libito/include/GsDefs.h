#ifndef _ITO_GSDEFS_H
#define _ITO_GSDEFS_H

/** Video RAM size in bytes. */
#define VRAM_SIZE					0x400000

/** General Purpose Registers
*/
#define GS_REG_PRIM					0x00
#define GS_REG_RGBAQ				0x01
#define GS_REG_ST					0x02
#define GS_REG_UV					0x03
#define GS_REG_XYZF2				0x04
#define GS_REG_XYZ2					0x05
#define GS_REG_FOG					0x0A
#define GS_REG_XYZF3				0x0C
#define GS_REG_XYZ3					0x0D
#define GS_REG_PRMODECONT			0x1A
#define GS_REG_PRMODE				0x1B
#define GS_REG_TEXCLUT				0x1C
#define GS_REG_SCANMSK				0x22
#define GS_REG_TEXA					0x3B
#define GS_REG_FOGCOL				0x3D
#define GS_REG_TEXFLUSH				0x3F
#define GS_REG_DIMX					0x44
#define GS_REG_DTHE					0x45
#define GS_REG_COLCLAMP				0x46
#define GS_REG_PABE					0x49
#define GS_REG_BITBLTBUF			0x50
#define GS_REG_TRXPOS				0x51
#define GS_REG_TRXREG				0x52
#define GS_REG_TRXDIR				0x53
#define GS_REG_HWREG				0x54
#define GS_REG_SIGNAL				0x60
#define GS_REG_FINISH				0x61
#define GS_REG_LABEL				0x62

/** Registers for context 1
*/
#define GS_REG_TEX0_1				0x06
#define GS_REG_CLAMP_1				0x08
#define GS_REG_TEX1_1				0x14
#define GS_REG_TEX2_1				0x16
#define GS_REG_XYOFFSET_1			0x18
#define GS_REG_MIPTBP1_1			0x34
#define GS_REG_MIPTBP2_1			0x36
#define GS_REG_SCISSOR_1			0x40
#define GS_REG_ALPHA_1				0x42
#define GS_REG_TEST_1				0x47
#define GS_REG_FBA_1				0x4A
#define GS_REG_FRAME_1				0x4C
#define GS_REG_ZBUF_1				0x4E

/** Registers for context 2
*/
#define GS_REG_TEX0_2				0x07
#define GS_REG_CLAMP_2				0x09
#define GS_REG_TEX1_2				0x15
#define GS_REG_TEX2_2				0x17
#define GS_REG_XYOFFSET_2			0x19
#define GS_REG_MIPTBP1_2			0x35
#define GS_REG_MIPTBP2_2			0x37
#define GS_REG_SCISSOR_2			0x41
#define GS_REG_ALPHA_2				0x43
#define GS_REG_TEST_2				0x48
#define GS_REG_FBA_2				0x4B
#define GS_REG_FRAME_2				0x4D
#define GS_REG_ZBUF_2				0x4F

/** Same as context 1, nicer naming for auto context */
#define GS_REG_TEX0					GS_REG_TEX0_1
#define GS_REG_CLAMP				GS_REG_CLAMP_1
#define GS_REG_TEX1					GS_REG_TEX1_1
#define GS_REG_TEX2					GS_REG_TEX2_1
#define GS_REG_XYOFFSET				GS_REG_XYOFFSET_1
#define GS_REG_MIPTBP1				GS_REG_MIPTBP1_1
#define GS_REG_MIPTBP2				GS_REG_MIPTBP2_1
#define GS_REG_SCISSOR				GS_REG_SCISSOR_1
#define GS_REG_ALPHA				GS_REG_ALPHA_1
#define GS_REG_TEST					GS_REG_TEST_1
#define GS_REG_FBA					GS_REG_FBA_1
#define GS_REG_FRAME				GS_REG_FRAME_1
#define GS_REG_ZBUF					GS_REG_ZBUF_1

/* GIF Controler Registers */
#define GIF_REG_CTRL				*((volatile u32*)0x10003000)
#define GIF_REG_MODE				*((volatile u32*)0x10003010)
#define GIF_REG_STAT				*((volatile u32*)0x10003020)
#define GIF_REG_TAG0				*((volatile u32*)0x10003040)
#define GIF_REG_TAG1				*((volatile u32*)0x10003050)
#define GIF_REG_TAG2				*((volatile u32*)0x10003060)
#define GIF_REG_TAG3				*((volatile u32*)0x10003070)
#define GIF_REG_CNT					*((volatile u32*)0x10003080)
#define GIF_REG_P3CNT				*((volatile u32*)0x10003090)
#define GIF_REG_P3TAG				*((volatile u32*)0x100030A0)


/** Privileged Registers
*/

#define GS_REG_PMODE				*((volatile u64*)0x12000000)
#define GS_REG_SMODE2				*((volatile u64*)0x12000020)
#define GS_REG_DISPFB1				*((volatile u64*)0x12000070)
#define GS_REG_DISPLAY1				*((volatile u64*)0x12000080)
#define GS_REG_DISPFB2				*((volatile u64*)0x12000090)
#define GS_REG_DISPLAY2				*((volatile u64*)0x120000A0)
#define GS_REG_BGCOLOR				*((volatile u64*)0x120000E0)
#define GS_REG_CSR					*((volatile u64*)0x12001000)
#define GS_REG_BUSDIR				*((volatile u64*)0x12001040)
#define GS_REG_EXTWRITE				*((volatile u64*)0x120000D0)

#define GIF_FIFO					((volatile u128 *)(0x10006000))

/** GifTag */

#define GIFTAG_PACKED				0x0
#define GIFTAG_LIST					0x1
#define GIFTAG_IMAGE				0x2

#define	GIFTAG_PRIM					0
#define	GIFTAG_RGBAQ				1
#define	GIFTAG_ST					2
#define	GIFTAG_UV					3
#define	GIFTAG_XYZF2				4
#define	GIFTAG_XYZ2					5
#define	GIFTAG_TEX0_1				6
#define	GIFTAG_TEX0_2				7
#define	GIFTAG_CLAMP_1				8
#define	GIFTAG_CLAMP_2				9
#define	GIFTAG_FOG					10
#define	GIFTAG_XYZF3				12
#define	GIFTAG_XYZ3					13
#define GIFTAG_AD					14
#define GIFTAG_NOP					15

#define GIFTAG(nloop, eop, pre, prim, flag, nreg) \
			((u64)(nreg) << 60) | ((u64)(flag) << 58) | ((u64)(prim) << 47) \
			| ((u64)(pre) << 46) | ((u64)(eop) << 15) | (nloop)
		
inline u64 GIFTAG_REGS(	u8 r1 = 0, u8 r2 = 0, u8 r3 = 0, u8 r4 = 0, u8 r5 = 0, u8 r6 = 0, u8 r7 = 0, u8 r8 = 0, u8 r9 = 0, u8 r10 = 0, u8 r11 = 0, u8 r12 = 0, u8 r13 = 0, u8 r14 = 0, u8 r15 = 0, u8 r16 = 0 )
{
	return	( ((u64)r16 << 60) ) | \
			( ((u64)r15 << 56) ) | \
			( ((u64)r14 << 52) ) | \
			( ((u64)r13 << 48) ) | \
			( ((u64)r12 << 44) ) | \
			( ((u64)r11 << 40) ) | \
			( ((u64)r10 << 36) ) | \
			( ((u64)r9 << 32)  ) | \
			( ((u64)r8 << 28)  ) | \
			( ((u64)r7 << 24)  ) | \
			( ((u64)r6 << 20)  ) | \
			( ((u64)r5 << 16)  ) | \
			( ((u64)r4 << 12)  ) | \
			( ((u64)r3 << 8)   ) | \
			( ((u64)r2 << 4)   ) | \
			( ((u64)r1) ) ;
}
/** General Purpose Registers */

#define GS_PRMODECONT(prmodecont) prmodecont

#define GS_ZBUF(zb_base, psm, update) \
			((s64)(update) << 32 | (s64)(psm) << 24 | (s64)(zb_base) >> 13)

#define GS_FRAME(fb_base, fb_width, psm, update) \
			((((u64)(update) << 32) | ((u64)(psm) << 24) | (((fb_width) >> 6) << 16) | ((fb_base) >> 13)))
	
#define GS_SCISSOR(scissor_x1, scissor_y1, scissor_x2, scissor_y2) \
			((s64)(scissor_y2) << 48 | (s64)(scissor_y1) << 32 | \
			(s64)(scissor_x2) << 16 | (scissor_x1))

#define GS_TEST( ate, atst, aref, afail, date, datm, zte, ztst) \
			((s64)(ztst) << 17 | (s64)(zte) << 16 | (s64)(datm) << 15 | (s64)(date) << 14 | \
			(s64)(afail) << 12 | (s64)(aref) << 4 | (s64)(atst) << 1 | (s64)ate)

#define GS_XYOFFSET(offset_x, offset_y) \
			(s64)(offset_y) << 32 | (offset_x)

#define	GS_PRIM(prim, iip, tme, fge, abe, aai, fst, ctxt, fix) \
			(fix) << 10 | (ctxt) << 9 | (fst) << 8 | (aai) << 7 \
			| (abe) << 6 | (fge) << 5 | (tme) << 4 | (iip) << 3 | (prim)

#define	GS_PRIM2(prim, iip, tme, mode) \
			(mode) | (tme) << 4 | (iip) << 3 | (prim)

#define GS_ALPHA(a, b, c, d, fix) \
			((u64)fix << 32) | ((u64)d << 6) | ((u64)c << 4) | ((u64)b << 2)| ((u64)a)

#define GS_TEX0(tbp0, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld) \
			(tbp0/256) | (s64)((tbw)/64) << 14 | (s64)(psm) << 20 \
			| (s64)(tw) << 26 | (s64)(th) << 30 | (s64)(tcc) << 34 \
			| (s64)(tfx) << 35 | (s64)((cbp) / 256) << 37 | (s64)(cpsm) << 51 \
			| (s64)(csm) << 55 | (s64)((csa)/16) << 56 | (s64)(cld) << 61 

#define GS_NOP	0

#define GS_XYZ2(x, y, z) ((s64)(z) << 32|(s64)(y) << 16 | (s64)(x))

#define GS_XYZ3(x, y, z) ((s64)(z) << 32|(s64)(y) << 16 | (s64)(x))

#define GS_RGBAQ(rgbaq) rgbaq

#define GS_UV(u, v)  ((s64)(v) << 16) | ((s64)(u))

#define GS_BITBLTBUF(sbp, sbw, spsm, dbp, dbw, dpsm) \
			(sbp) >> 8 | (s64)((sbw) >> 6) << 16 | (s64)(spsm) << 24 \
			| (s64)((dbp) >> 8) << 32 | (s64)((dbw) >> 6) << 48 | (s64)(dpsm) << 56

#define GS_TRXPOS(src_x, src_y, dst_x, dst_y, dir) \
			((s64)(src_y)) << 16 | ((s64)(dst_x)) << 32 \
			| ((s64)(dst_y)) << 48 | ((s64)(dir)) << 59

#define GS_TRXDIR(dir) dir

#define GS_TRXREG(width, height) \
			(width) | (s64)(height) << 32

#define GS_TEXFLUSH() 0



// Primitive Settings
#define PRIM_GOURAUD			(1 << 3)
#define PRIM_FLAT				0
#define PRIM_TEXTURE			(1 << 4)
#define PRIM_FOG				(1 << 5)
#define PRIM_ALPHABLEND			(1 << 6)
#define PRIM_ANTIALIAS			(1 << 7)
#define PRIM_UV					(1 << 8)
#define PRIM_STQ				0
#define PRIM_CONTEXT_1			0
#define PRIM_CONTEXT_2			(1 << 9)

#define PRIM_SHADE_FLAT			0
#define PRIM_SHADE_GOURAUD		1

#define PRIM_TEXCOORDS_UV		1
#define PRIM_TEXCOORDS_STQ		0

#define PRIM_POINT				0
#define PRIM_LINE				1
#define PRIM_LINE_STRIP			2
#define PRIM_TRIANGLE			3
#define PRIM_TRIANGLE_STRIP		4
#define PRIM_TRIANGLE_FAN		5
#define PRIM_SPRITE				6

//-----------------------------------------------------------------------------
// Pixel Storage Mode (PSM) defines & macros.
//-----------------------------------------------------------------------------

#define RGBA5551(r,g,b,a)			(((a) << 15)|((b) << 10)|((g) << 5)|(r))

#define RGB(r,g,b)					(((b) << 16) | ((g) << 8) | (r))
#define RGBA(r,g,b,a)				(((a) << 24) | ((b) << 16) | ((g) << 8) | (r))
#define RGBAQ(r,g,b,a,q)			(((u64)(q) << 32)|((a) << 24)|((b) << 16)|((g) << 8)|(r))
#define Q(q)						((u64)(*(u64*)(q)) << 32) | 0

#define	RGBA32						0
#define RGB24						1
#define RGBA16						2
#define RGBA16S						10
#define CLUT8						19
#define CLUT4						20

#define ZBUF32						0
#define ZBUF24						1
#define ZBUF16						2
#define ZBUF16S						6
#define NOZBUF						32

//-----------------------------------------------------------------------------
// Alpha Blending
//-----------------------------------------------------------------------------

#define ALPHA_COLOR_SRC				0
#define ALPHA_COLOR_DST				1
#define ALPHA_ZERO					2

#define ALPHA_VALUE_SRC				0
#define ALPHA_VALUE_DST				1
#define ALPHA_FIXED					2

//-----------------------------------------------------------------------------
// Alpha Test
//-----------------------------------------------------------------------------

#define ALPHA_TEST_NEVER			0
#define ALPHA_TEST_ALWAYS			1
#define ALPHA_TEST_LESS				2
#define ALPHA_TEST_LEQUAL			3
#define ALPHA_TEST_EQUAL			4
#define ALPHA_TEST_GEQUAL			5
#define ALPHA_TEST_GREATER			6
#define ALPHA_TEST_NOTEQUAL			7

//-----------------------------------------------------------------------------
// Alpha Test Failed
//-----------------------------------------------------------------------------

#define ALPHA_FAIL_KEEP				0
#define ALPHA_FAIL_FB_ONLY			1
#define ALPHA_FAIL_ZB_ONLY			2
#define ALPHA_FAIL_RGB_ONLY			3

//-----------------------------------------------------------------------------
// ZBuffer Test
//-----------------------------------------------------------------------------

#define ZTEST_NEVER					0
#define ZTEST_ALWAYS				1
#define ZTEST_GEQUAL				2
#define ZTEST_GREATER				3

/** Video Setup. */
#define	VMODE_NTSC					2
#define	VMODE_PAL					3
#define VMODE_AUTO					(((*((u8*)0x1FC7FF52))=='E')+2)
#define	FRAME						1
#define FIELD						0

/** PRMODECONT SETTINGS */
#define GS_PRMODECONT_PRIM			1
#define GS_PRMODECONT_REGISTER		0

#endif

