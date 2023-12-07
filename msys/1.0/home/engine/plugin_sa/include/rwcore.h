#ifndef RWCORE_H_
#define RWCORE_H_

#include <stdint.h>
#include <CVector.h>

#include <eestruct.h>
#include <eeregs.h>

#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long

typedef float RwReal;
typedef unsigned int RwUInt32;
typedef unsigned short RwUInt16;
typedef unsigned char RwUInt8;
typedef int RwInt32;
typedef short RwInt16;

typedef long RwInt64;
typedef unsigned long RwUInt64;
typedef __int128_t RwInt128;
typedef __uint128_t RwUInt128;

#define RWALIGN(type, x)   type __attribute__((aligned(x)))


#define RWFORCEENUMSIZEINT ((RwInt32)((~((RwUInt32)0))>>1))


#define GS_NOINTERLACE		(0)
#define GS_INTERLACE		(1)
#define	GS_FIELD			(0)
#define	GS_FRAME			(1)
#define GS_NTSC			(0x2)
#define GS_PAL			(0x3)
#define GS_VESA1A			(0x1a)    /* System Use */
#define GS_VESA1B			(0x1b)    /* System Use */
#define GS_VESA1C			(0x1c)    /* System Use */
#define GS_VESA1D			(0x1d)    /* System Use */
#define GS_VESA2A			(0x2a)    /* System Use */
#define GS_VESA2B			(0x2b)    /* System Use */
#define GS_VESA2C			(0x2c)    /* System Use */
#define GS_VESA2D			(0x2d)    /* System Use */
#define GS_VESA2E			(0x2e)    /* System Use */
#define GS_VESA3B			(0x3b)    /* System Use */
#define GS_VESA3C			(0x3c)    /* System Use */
#define GS_VESA3D			(0x3d)    /* System Use */
#define GS_VESA3E			(0x3e)    /* System Use */
#define GS_VESA4A			(0x4a)    /* System Use */
#define GS_VESA4B			(0x4b)    /* System Use */
#define GS_DTV480P			(0x50)
#define GS_PSMCT32			(0)
#define GS_PSMCT24			(1)
#define GS_PSMCT16			(2)
#define GS_PSMCT16S			(10)
#define GS_PSMT8			(19)
#define GS_PSMT4			(20)
#define GS_PSMT8H			(27)
#define GS_PSMT4HL			(36)
#define GS_PSMT4HH			(44)
#define GS_PSMZ32			(48)
#define GS_PSMZ24			(49)
#define GS_PSMZ16			(50)
#define GS_PSMZ16S			(58)
#define GS_ZNOUSE			(0)
#define GS_ZALWAYS			(1)
#define GS_ZGEQUAL			(2)
#define GS_ZGREATER			(3)
#define GS_NOCLEAR			(0)
#define GS_CLEAR			(1)
#define GS_MODULATE			(0)
#define GS_DECAL			(1)
#define GS_HILIGHT			(2)
#define GS_GHLIGHT2   		GS_HIGHLIGHT2
#define GS_HIGHLIGHT2		(3)
#define GS_NEAREST			(0)
#define GS_LINEAR			(1)
#define GS_NEAREST_MIPMAP_NEAREST	(2)
#define GS_NEAREST_MIPMAP_LINEAR	GS_NEAREST_MIPMAP_LENEAR
#define GS_NEAREST_MIPMAP_LENEAR	(3)
#define GS_LINEAR_MIPMAP_NEAREST	(4)
#define GS_LINEAR_MIPMAP_LINEAR	(5)
#define GS_PRIM_POINT		(0)
#define GS_PRIM_LINE		(1)
#define GS_PRIM_LINESTRIP		(2)
#define GS_PRIM_TRI			(3)
#define GS_PRIM_TRISTRIP		(4)
#define GS_PRIM_TRIFAN		(5)
#define GS_PRIM_SPRITE		(6)
#define GS_PRIM_IIP			(1<<3)
#define GS_PRIM_TME			(1<<4)
#define GS_PRIM_FGE			(1<<5)
#define GS_PRIM_ABE			(1<<6)
#define GS_PRIM_AA1			(1<<7)
#define GS_PRIM_FST			(1<<8)
#define GS_PRIM_CTXT1		(0)
#define GS_PRIM_CTXT2		(1<<9)
#define GS_PRIM_FIX			(1<<10)

#define	GS_FALSE			(0)
#define	GS_TRUE			(1)

#define GS_REPEAT			(0)
#define GS_CLAMP			(1)
#define GS_REGION_CLAMP		(2)
#define GS_REGION_REPEAT		(3)

#define GS_DEPTH_NEVER		(0)
#define GS_DEPTH_ALWAYS		(1)
#define GS_DEPTH_GEQUAL		(2)
#define GS_DEPTH_GREATER		(3)

#define GS_ALPHA_NEVER		(0)
#define GS_ALPHA_ALWAYS		(1)
#define GS_ALPHA_LESS		(2)
#define GS_ALPHA_LEQUAL		(3)
#define GS_ALPHA_EQUAL		(4)
#define GS_ALPHA_GEQUAL		(5)
#define GS_ALPHA_GREATER		(6)
#define GS_ALPHA_NOTEQUAL		(7)

#define	GS_AFAIL_KEEP		(0)
#define	GS_AFAIL_FB_ONLY		(1)
#define GS_AFAIL_ZB_ONLY		(2)
#define GS_AFAIL_RGB_ONLY		(3)

#define GS_ALPHA_CS			(0)
#define GS_ALPHA_CD			(1)
#define GS_ALPHA_ZERO		(2)
#define GS_ALPHA_AS			(0)
#define GS_ALPHA_AD			(1)
#define GS_ALPHA_FIX		(2)

typedef struct {
	tGS_PMODE	pmode;
	tGS_SMODE2	smode2;
	tGS_DISPFB2	dispfb;
	tGS_DISPLAY2	display;
	tGS_BGCOLOR	bgcolor;
} sceGsDispEnv;

typedef struct {
	sceGsFrame	frame1;
	uint32_t		frame1addr;
	sceGsZbuf	zbuf1;
	long		zbuf1addr;
	sceGsXyoffset	xyoffset1;
	long		xyoffset1addr;
	sceGsScissor	scissor1;
	long		scissor1addr;
	sceGsPrmodecont	prmodecont;
	long		prmodecontaddr;
	sceGsColclamp	colclamp;
	long		colclampaddr;
	sceGsDthe	dthe;
	long		dtheaddr;
	sceGsTest	test1;
	long		test1addr;
} sceGsDrawEnv1 __attribute__((aligned(16)));

typedef struct {
	sceGsFrame	frame2;
	uint32_t		frame2addr;
	sceGsZbuf	zbuf2;
	long		zbuf2addr;
	sceGsXyoffset	xyoffset2;
	long		xyoffset2addr;
	sceGsScissor	scissor2;
	long		scissor2addr;
	sceGsPrmodecont	prmodecont;
	long		prmodecontaddr;
	sceGsColclamp	colclamp;
	long		colclampaddr;
	sceGsDthe	dthe;
	long		dtheaddr;
	sceGsTest	test2;
	long		test2addr;
} sceGsDrawEnv2 __attribute__((aligned(16)));

typedef struct {
	sceGsTest	testa;
	long		testaaddr;
	sceGsPrim	prim;
	long		primaddr;
	sceGsRgbaq	rgbaq;
	long		rgbaqaddr;
	sceGsXyz	xyz2a;
	long		xyz2aaddr;
	sceGsXyz	xyz2b;
	long		xyz2baddr;
	sceGsTest	testb;
	long		testbaddr;
} sceGsClear __attribute__((aligned(16)));

typedef struct {
	sceGsDispEnv	disp[2];
	sceGifTag	giftag0;
	sceGsDrawEnv1	draw0;
	sceGsClear	clear0;
	sceGifTag	giftag1;
	sceGsDrawEnv1	draw1;
	sceGsClear	clear1;
} sceGsDBuff;

typedef struct {
	sceGsDispEnv	disp[2];
	sceGifTag	giftag0;
	sceGsDrawEnv1	draw01;
	sceGsDrawEnv2	draw02;
	sceGsClear	clear0;
	sceGifTag	giftag1;
	sceGsDrawEnv1	draw11;
	sceGsDrawEnv2	draw12;
	sceGsClear	clear1;
} sceGsDBuffDc;

typedef struct {
	short		sceGsInterMode;
	short		sceGsOutMode;
	short		sceGsFFMode;
	short		sceGsVersion;
	int		(*sceGsVSCfunc)(int);
	int		sceGsVSCid;
} sceGsGParam __attribute__((aligned(16)));

#if (!defined(rwMATRIXALIGNMENT))
#define rwMATRIXALIGNMENT sizeof(RwUInt128)
#endif /* (!defined(rwMATRIXALIGNMENT)) */

#if (!defined(rwFRAMEALIGNMENT))
#define rwFRAMEALIGNMENT sizeof(RwUInt128)
#endif /* (!defined(rwFRAMEALIGNMENT)) */

#if (!defined(rwV4DALIGNMENT))
#define rwV4DALIGNMENT sizeof(RwUInt128)
#endif /* (!defined(rwV4DALIGNMENT)) */

#if (!defined(rwMALLOCALIGNMENT))
#define rwMALLOCALIGNMENT sizeof(RwUInt128)
#endif /* (!defined(rwMALLOCALIGNMENT) */


enum RwVideoModeFlag
{
    rwVIDEOMODEEXCLUSIVE    = 0x0001, /**<Exclusive (i.e. full-screen) */
    rwVIDEOMODEINTERLACE    = 0x0002, /**<Interlaced                   */
    rwVIDEOMODEFFINTERLACE  = 0x0004, /**<Flicker Free Interlaced      */

    /* Platform specific video mode flags. */

    rwVIDEOMODE_PS2_FSAASHRINKBLIT    = 0x0100,
    /**< \if sky2
     *   Full-screen antialiasing mode 0
     *   \endif
     */
    rwVIDEOMODE_PS2_FSAAREADCIRCUIT   = 0x0200,
    /**< \if sky2
     *   Full-screen antialiasing mode 1
     *   \endif
     */

    rwVIDEOMODE_XBOX_WIDESCREEN       = 0x0100,
    /**< \if xbox
     *   Wide screen.
     *   \endif
     */
    rwVIDEOMODE_XBOX_PROGRESSIVE      = 0x0200,
    /**< \if xbox
     *   Progressive.
     *   \endif
     */
    rwVIDEOMODE_XBOX_FIELD            = 0x0400,
    /**< \if xbox
     *   Field rendering.
     *   \endif
     */
    rwVIDEOMODE_XBOX_10X11PIXELASPECT = 0x0800,
    /**< \if xbox
     *   The frame buffer is centered on the display.
     *   On a TV that is 704 pixels across, this would leave 32 pixels of black
     *   border on the left and 32 pixels of black border on the right.
     *   \endif
     */

    rwVIDEOMODEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwVideoModeFlag RwVideoModeFlag;

#define rwVIDEOMODEFSAA0 rwVIDEOMODE_PS2_FSAASHRINKBLIT
#define rwVIDEOMODEFSAA1 rwVIDEOMODE_PS2_FSAAREADCIRCUIT


typedef struct RwVideoMode RwVideoMode;
struct RwVideoMode
{
        RwInt32         width;   /**< Width  */
        RwInt32         height;  /**< Height */
        RwInt32         depth;   /**< Depth  */
        RwVideoModeFlag flags;   /**< Flags  */
        RwInt32         refRate; /**< Approximate refresh rate */
        RwInt32         format;  /**< Raster format
                                  * \see RwRasterFormat 
                                  */
};

typedef struct
{
    /* These are padded to be 16 byte quantities per line */
    CVector             right;
    uint32_t            flags;
    CVector             up;
    uint32_t            pad1;
    CVector             at;
    uint32_t            pad2;
    CVector             pos;
    uint32_t            pad3;
} RwMatrix;

typedef struct RwRaster RwRaster;

struct RwRaster
{
    RwRaster           *parent; /* Top level raster if a sub raster */
    RwUInt8            *cpPixels; /* Pixel pointer when locked */
    RwUInt8            *palette; /* Raster palette */
    RwInt32             width, height, depth; /* Dimensions of raster */
    RwInt32             stride; /* Lines bytes of raster */
    RwInt16             nOffsetX, nOffsetY; /* Sub raster offset */
    RwUInt8             cType;  /* Type of raster */
    RwUInt8             cFlags; /* Raster flags */
    RwUInt8             privateFlags; /* Raster private flags */
    RwUInt8             cFormat; /* Raster format */

    RwUInt8            *originalPixels;
    RwInt32             originalWidth;
    RwInt32             originalHeight;
    RwInt32             originalStride;
};

typedef struct RwLLLink RwLLLink;

struct RwLLLink
{
  RwLLLink *next;
  RwLLLink *prev;
};

typedef struct
{
  RwRaster *raster;
  int dict;
  RwLLLink lInDictionary;
  char name[32];
  char mask[32];
  int filterAddressing;
  int refCount;
} RwTexture;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RwRGBA;


extern float* RecipNearClip;
extern float* NearScreenZ;

#define RWRGBALONG(r,g,b,a) ((RwUInt32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

typedef struct RwSky2DVertexFields RwSky2DVertexFields;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} RwRGBAReal;

typedef struct
{
    float x;
    float y;
} CVector2D;

struct RwSky2DVertexFields
{
    CVector        scrVertex;                /**< Object space vertex */
    RwReal         camVertex_z;            /**< Camera space z (for fog table) */
    RwReal         u;                      /**< Texture coordinate */
    RwReal         v;                      /**< Texture coordinate */
    RwReal         recipZ;                 /**< Recipz */
    RwReal         pad1;                   /**< Padding (fog table) */
    RwRGBAReal     color;                  /**< Color */
    CVector        objNormal;              /**< We allocate this, even though
                                            * we don't use it - we may want
                                            * to later.
                                            */
    RwReal         pad2;                   /**< Alignment padding */
};

typedef union RwSky2DVertexAlignmentOverlay RwSky2DVertexAlignmentOverlay;

union RwSky2DVertexAlignmentOverlay
{
    RwSky2DVertexFields els;                            /**< Internal use */
    __uint128_t qWords[4];                                /**< Internal use */
};

typedef struct RwSky2DVertex RwSky2DVertex;

struct RwSky2DVertex
{
    RwSky2DVertexAlignmentOverlay u;                    /**< Internal use */
};

typedef RwSky2DVertex  RwIm2DVertex;

typedef struct
{
  CVector pos;
  RwRGBA color;
  CVector objNormal;
  float u;
  float v;
} RxObjSpace3dVertex;

typedef RwUInt16        RxVertexIndex;
typedef RxVertexIndex   RwImVertexIndex;


typedef struct RwRect RwRect;

struct RwRect
{
    RwInt32 x;  /**< X value of the top-left corner */
    RwInt32 y;  /**< Y value of the top-left corner */
    RwInt32 w;  /**< Width of the rectangle */
    RwInt32 h;  /**< Height of the rectangle */
};

enum RwPrimitiveType
{
    rwPRIMTYPENAPRIMTYPE = 0,   /**<Invalid primative type */
    rwPRIMTYPELINELIST = 1,     /**<Unconnected line segments, each line is specified by
                                 * both its start and end index, independently of other lines
                                 * (for example, 3 segments specified as 0-1, 2-3, 4-5) */
    rwPRIMTYPEPOLYLINE = 2,     /**<Connected line segments, each line's start index
                                 * (except the first) is specified by the index of the end of
                                 * the previous segment (for example, 3 segments specified as
                                 * 0-1, 1-2, 2-3) */
    rwPRIMTYPETRILIST = 3,      /**<Unconnected triangles: each triangle is specified by
                                 * three indices, independently of other triangles (for example,
                                 * 3 triangles specified as 0-1-2, 3-4-5, 6-7-8) */
    rwPRIMTYPETRISTRIP = 4,     /**<Connected triangles sharing an edge with, at most, one
                                 * other forming a series (for example, 3 triangles specified
                                 * as 0-2-1, 1-2-3-, 2-4-3) */
    rwPRIMTYPETRIFAN = 5 ,      /**<Connected triangles sharing an edge with, at most,
                                 * two others forming a fan (for example, 3 triangles specified
                                 * as 0-2-1, 0-3-2, 0-4-3) */
    rwPRIMTYPEPOINTLIST = 6,    /**<Points 1, 2, 3, etc. This is not
                                 * supported by the default RenderWare
                                 * immediate or retained-mode pipelines
                                 * (except on PlayStation 2), it is intended
                                 * for use by user-created pipelines */
    rwPRIMITIVETYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

enum RwRenderState
{
    rwRENDERSTATENARENDERSTATE = 0,

    rwRENDERSTATETEXTURERASTER,
        /**<Raster used for texturing (normally used in immediate mode). 
         *  The value is a pointer to an \ref RwRaster.
         * Default: NULL.
         */
    rwRENDERSTATETEXTUREADDRESS,
        /**<\ref RwTextureAddressMode: wrap, clamp, mirror or border.
         * Default: rwTEXTUREADDRESSWRAP.
         */
    rwRENDERSTATETEXTUREADDRESSU,
        /**<\ref RwTextureAddressMode in u only.
         * Default: rwTEXTUREADDRESSWRAP.
         */
    rwRENDERSTATETEXTUREADDRESSV,
        /**<\ref RwTextureAddressMode in v only.
         * Default: rwTEXTUREADDRESSWRAP.
         */
    rwRENDERSTATETEXTUREPERSPECTIVE,
        /**<Perspective correction on/off (always enabled on many platforms).
         */
    rwRENDERSTATEZTESTENABLE,
        /**<Z-buffer test on/off.
         * Default: TRUE.
         */
    rwRENDERSTATESHADEMODE,
        /**<\ref RwShadeMode: flat or gouraud shading.
         * Default: rwSHADEMODEGOURAUD.
         */
    rwRENDERSTATEZWRITEENABLE,
        /**<Z-buffer write on/off.
         * Default: TRUE.
         */
    rwRENDERSTATETEXTUREFILTER,
        /**<\ref RwTextureFilterMode: point sample, bilinear, trilinear, etc.
         * Default: rwFILTERLINEAR.
         */
    rwRENDERSTATESRCBLEND,
        /**<\ref RwBlendFunction used to modulate the source pixel color
         *  when blending to the frame buffer.
         * Default: rwBLENDSRCALPHA.
         */
    rwRENDERSTATEDESTBLEND,
        /**<\ref RwBlendFunction used to modulate the destination pixel
         *  color in the frame buffer when blending. The resulting pixel
         *  color is given by the formula 
         *  (SRCBLEND * srcColor + DESTBLEND * destColor) for each RGB
         *  component. For a particular platform, not all combinations
         *  of blend function are allowed (see platform specific
         *  restrictions).
         * Default: rwBLENDINVSRCALPHA.
         */
    rwRENDERSTATEVERTEXALPHAENABLE,
        /**<Alpha blending on/off (always enabled on some platforms). 
         *  This is normally used in immediate mode to enable alpha blending 
         *  when vertex colors or texture rasters have transparency. Retained
         *  mode pipelines will usually set this state based on material colors 
         *  and textures.
         * Default: FALSE.
         */
    rwRENDERSTATEBORDERCOLOR,
        /**<Border color for \ref RwTextureAddressMode 
         *  \ref rwTEXTUREADDRESSBORDER. The value should be a packed 
         *  RwUInt32 in a platform specific format. The macro 
         *  RWRGBALONG(r, g, b, a) may be used to construct this using 
         *  8-bit color components.
         * Default: RWRGBALONG(0, 0, 0, 0).
         */
    rwRENDERSTATEFOGENABLE,
        /**<Fogging on/off (all polygons will be fogged).  
         * Default: FALSE.
         */
    rwRENDERSTATEFOGCOLOR,
        /**<Color used for fogging. The value should be a packed RwUInt32 
         *  in a platform specific format. The macro RWRGBALONG(r, g, b, a)
         *  may be used to construct this using 8-bit color components. 
         * Default: RWRGBALONG(0, 0, 0, 0).
         */
    rwRENDERSTATEFOGTYPE,
        /**<\ref RwFogType, the type of fogging to use. 
         * Default: rwFOGTYPELINEAR.
         */
    rwRENDERSTATEFOGDENSITY,
        /**<Fog density for \ref RwFogType of 
         *  \ref rwFOGTYPEEXPONENTIAL or \ref rwFOGTYPEEXPONENTIAL2. 
         *  The value should be a pointer to an RwReal in the 
         *  range 0 to 1.  
         * Default: 1.
         */
    rwRENDERSTATECULLMODE = 20,
        /**<\ref RwCullMode, for selecting front/back face culling, or
         *  no culling.
         * Default: rwCULLMODECULLBACK.
         */
    rwRENDERSTATESTENCILENABLE,
        /**<Stenciling on/off.
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: FALSE.
         */
    rwRENDERSTATESTENCILFAIL,
        /**<\ref RwStencilOperation used when the stencil test passes.
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: rwSTENCILOPERATIONKEEP.
         */
    rwRENDERSTATESTENCILZFAIL,
        /**<\ref RwStencilOperation used when the stencil test passes and 
         *  the depth test (z-test) fails. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: rwSTENCILOPERATIONKEEP.
         */
    rwRENDERSTATESTENCILPASS,
        /**<\ref RwStencilOperation used when both the stencil and the depth 
         *  (z) tests pass. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: rwSTENCILOPERATIONKEEP.
         */
    rwRENDERSTATESTENCILFUNCTION,
        /**<\ref RwStencilFunction for the stencil test. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: rwSTENCILFUNCTIONALWAYS.
         */
    rwRENDERSTATESTENCILFUNCTIONREF,
        /**<Integer reference value for the stencil test. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: 0.
         */
    rwRENDERSTATESTENCILFUNCTIONMASK,
        /**<Mask applied to the reference value and each stencil buffer 
         *  entry to determine the significant bits for the stencil test. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: 0xffffffff.
         */
    rwRENDERSTATESTENCILFUNCTIONWRITEMASK,
        /**<Write mask applied to values written into the stencil buffer. 
         *  <i> Supported on Xbox, D3D8, D3D9, and OpenGL only. </i>
         * Default: 0xffffffff.
         */
    rwRENDERSTATEALPHATESTFUNCTION,
        /**<\ref RwAlphaTestFunction for the alpha test. When a pixel fails,
         * neither the frame buffer nor the Z-buffer are updated.
         * Default: rwALPHATESTFUNCTIONGREATER (GameCube, Xbox, D3D8, D3D9
         * and OpenGL). The default PS2 behaviour is to always update the
         * frame buffer and update the Z-buffer only if a greater than or
         * equal test passes.
         */
    rwRENDERSTATEALPHATESTFUNCTIONREF,
        /**<Integer reference value for the alpha test. 
         *  <i> Range is 0 to 255, mapped to the platform's actual range </i>
         * Default: 128 (PS2) 0 (GameCube, Xbox, D3D8, D3D9 and OpenGL).
         */

    rwRENDERSTATEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

enum RwRasterType
{
    rwRASTERTYPENORMAL = 0x00,          /**<Normal */
    rwRASTERTYPEZBUFFER = 0x01,         /**<Z Buffer */
    rwRASTERTYPECAMERA = 0x02,          /**<Camera */
    rwRASTERTYPETEXTURE = 0x04,         /**<Texture */
    rwRASTERTYPECAMERATEXTURE = 0x05,   /**<Camera texture */
    rwRASTERTYPEMASK = 0x07,            /**<Mask for finding type */

    rwRASTERDONTALLOCATE = 0x80,        /**<If set the raster is not allocated */
    rwRASTERTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

enum RwTextureFilterMode
{
    rwFILTERNAFILTERMODE = 0,
    rwFILTERNEAREST,                /**<Point sampled */
    rwFILTERLINEAR,                 /**<Bilinear */
    rwFILTERMIPNEAREST,             /**<Point sampled per pixel mip map */
    rwFILTERMIPLINEAR,              /**<Bilinear per pixel mipmap */
    rwFILTERLINEARMIPNEAREST,       /**<MipMap interp point sampled */
    rwFILTERLINEARMIPLINEAR,        /**<Trilinear */
    rwTEXTUREFILTERMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwTextureFilterMode RwTextureFilterMode;

typedef enum RwRasterType RwRasterType;


extern void (*psCameraShowRaster)(void*);
extern void (*RwIm2DRenderPrimitive)(RwInt32, RwIm2DVertex*, RwInt32);
extern int (*RwRenderStateSet)(RwInt32, void*);
extern void (*RwIm3DRenderIndexedPrimitive)(int, RwImVertexIndex*, RwInt32);
extern void  *(*RwIm3DTransform)(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags);
extern void (*RwIm3DEnd)();
extern void (*RwIm2DRenderIndexedPrimitive)(int, RwIm2DVertex*, RwInt32, RwImVertexIndex*, RwInt32);
extern RwRaster* (*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
extern int (*RwRasterDestroy)(RwRaster * raster);
extern RwRaster* (*RwRasterSubRaster)(RwRaster* subRaster, RwRaster* raster, RwRect* rect);
extern RwUInt8 *(*RwRasterLock)(RwRaster * raster, RwUInt8 level, RwInt32 lockMode);
extern RwRaster *(*RwRasterUnlock)(RwRaster * raster);
extern RwTexture* (*RwTextureCreate)(RwRaster * raster);
extern int (*RwTextureDestroy)(RwTexture * texture);
extern void (*RpSkyTexCacheRasterLock)(RwRaster *raster, int bLocked);

extern void (*RwRasterPushContext)(RwRaster* raster);
extern void (*RwRasterPopContext)();

extern RwRaster *(*RwRasterRenderFast)(RwRaster * raster, RwInt32 x, RwInt32 y);
extern RwRaster *(*RwRasterRender)(RwRaster * raster, RwInt32 x, RwInt32 y);
extern RwRaster *(*RwRasterRenderScaled)(RwRaster * raster, RwRect * rect);

extern void *(*RwCameraBeginUpdate)(void * camera);
extern void *(*RwCameraEndUpdate)(void * camera);

extern void *(*RwCameraClear)(void *camera, RwRGBA *colour, RwInt32 clearMode);

extern RwRaster *(*RpSkyTexSetTex0)(RwRaster *r, RwUInt32 msb, RwUInt32 lsb);
extern RwRaster *(*RpSkyTexGetTex0)(RwRaster *r, RwUInt32 *msb, RwUInt32 *lsb);

extern RwRaster *	(*RpSkyGetDisplayBufferRaster)(void);
extern RwRaster *	(*RpSkyGetDrawBufferRaster)(void);

typedef struct RwFileFunction{
	int padding;  
	void* (*rwfopen)(const char*, const char*);
	int (*rwfclose)(void *);
	size_t (*rwfread)(void *, void *, size_t);   
	size_t (*rwfwrite)(void *, const void *, size_t);
	char* (*rwfgets)(char *, int, void *);
	int (*rwfputs)(int, int); 
	int (*rwfeof)(void*);   
	int (*rwfseek)(void *, long int, int);  
	int (*rwfflush)(void*);
	int (*rwftell)(void*);  
} RwFileFunction;

extern RwFileFunction* (*RwOsGetFileInterface)(void);

#   define MAKE128(RES, MSB, LSB) \
    __asm volatile ( "pcpyld %0, %1, %2" : "=r" (RES) : "r" (MSB), "r" (LSB))

#define _rwDMAPktPtr (*(__uint128_t**)0x66C170)

extern int (*_rwDMAOpenGIFPkt)(RwUInt32, RwUInt32);
extern int (*_rwDMAOpenVIFPkt)(RwUInt32, RwUInt32);

#define RWDMA_FIXUP     0x80000000

#define GIF_AD 0x0E
#define GS_TEST_1     0x47
#define GS_PABE       0x49
#define GS_SCISSOR_1    0x40
#define GS_ALPHA_1    0x42

#define GIF_TAG(NLOOP,EOP,PRE,PRIM,FLG,NREG) \
                ((unsigned long int)(NLOOP)	<< 0)	| \
                ((unsigned long int)(EOP)	<< 15)	| \
                ((unsigned long int)(PRE)	<< 46)	| \
                ((unsigned long int)(PRIM)	<< 47)	| \
                ((unsigned long int)(FLG)	<< 58)	| \
                ((unsigned long int)(NREG)	<< 60);


#define ADDTOPKT(_a, _b) { __uint128_t _c; MAKE128(_c, _a, _b); *_rwDMAPktPtr++ = _c; }


extern uint32_t* skyFrameBit;

extern int (*sceGsSyncV)(int mode);
extern void (*sceGsResetGraph)(short mode, short inter, short omode, short ffmode);
extern int (*sceGsSwapDBuffDc)(void *db, int id);
extern int (*sceGsSyncPath)(int mode, uint16_t timeout);
extern void *(*sceDmaGetChan)(int id);
extern void (*sceDmaSendN)(void *d, void *addr, int size);

extern void (*setInitialRenderState)(void);

extern void (*RpSkyTexCacheFlush)(void);

extern bool (*_rwDMAHook)();

extern bool* skySuspended;

extern void *_rwDMAFlipData;

extern void* skyInitialClear;

extern RwInt32 (*RwEngineGetNumVideoModes)(void);
extern RwVideoMode *(*RwEngineGetVideoModeInfo)(RwVideoMode *modeinfo, RwInt32 modeIndex);
extern RwInt32 (*RwEngineGetCurrentVideoMode)(void);
extern bool (*RwEngineSetVideoMode)(RwInt32 modeIndex);

extern void (*RsRwInitialize)(void*);
extern void (*RsRwTerminate)(void);

extern void (*SkyStart)();
extern uint32_t *currentMode;

extern void (*RpSkySuspend)();

extern void (*sceGsSetDefDBuffDc)(void *db, short psm, short w, short h, short ztest, short zpsm, short clear);




#endif
