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

typedef unsigned int RwUInt32;
typedef unsigned char RwUInt8;
typedef int RwInt32;


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
    short             nOffsetX, nOffsetY; /* Sub raster offset */
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

typedef struct RwLinkList RwLinkList;
struct RwLinkList
{
    RwLLLink link;
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


typedef struct RwObject RwObject;

struct RwObject
{
        uint8_t type;                /**< Internal Use */
        uint8_t subType;             /**< Internal Use */
        uint8_t flags;               /**< Internal Use */
        uint8_t privateFlags;        /**< Internal Use */
        void   *parent;              /**< Internal Use */
                                     /* Often a Frame  */
};

struct RwFrame
{
    RwObject            object;

    /* Put embedded matrics here to ensure they remain 16-byte aligned */
    RwMatrix            modelling;
    RwMatrix            ltm;

    RwLLLink            inDirtyListLink;

    RwLinkList          objectList; /* List of objects connected to a frame */

    struct RwFrame      *child;
    struct RwFrame      *next;
    struct RwFrame      *root;   /* Root of the tree */
};

typedef struct RwFrame RwFrame;


/* Type IDs */

#define rpATOMIC 1
#define rpCLUMP 2

/* Interpolator flags */
enum RpInterpolatorFlag
{
    rpINTERPOLATORDIRTYINSTANCE = 0x01,
    rpINTERPOLATORDIRTYSPHERE = 0x02,
    rpINTERPOLATORFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpInterpolatorFlag rpInterpolatorFlag;

/**
 * \ingroup rpworlddatatypes
 * The bit-field type RpAtomicFlag specifies the options available for
 * controlling the behavior of atomics. See API function \ref RpAtomicSetFlags.
 *
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpWorldSectorForAllCollisionAtomics
 */

enum RpAtomicFlag
{
    rpATOMICCOLLISIONTEST = 0x01, /**<A generic collision flag to indicate
                                   * that the atomic should be considered
                                   * in collision tests.
                                   */
    rpATOMICRENDER = 0x04,      /**<The atomic is rendered if it is
                                 * in the view frustum.
                                 */

    rpATOMICFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicFlag RpAtomicFlag;

/**
 * \ingroup rpworlddatatypes
 * \ref RpAtomicSetGeomFlag defines how an atomic references a new geometry
 *
 * \see RpAtomicSetGeometry
 */
enum RpAtomicSetGeomFlag
{
    rpATOMICSAMEBOUNDINGSPHERE = 0x01, /**<The bounding-sphere for the
                                        * new geometry is assumed to be the
                                        * same as the old one, if any, and
                                        * should not be recalculated.
                                        */

    rpATOMICSETGEOMFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicSetGeomFlag RpAtomicSetGeomFlag;

enum RpAtomicPrivateFlag
{
    rpATOMICPRIVATEWORLDBOUNDDIRTY = 0x01,
    rpATOMICPRIVATEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicPrivateFlag rpAtomicPrivateFlag;


/**
 * \ingroup rpworlddatatypes
 * \typedef RpAtomic
 * Atomic Geometry object. This should be
 * considered an opaque type. Use the RpAtomic API functions to access.
 */
typedef struct RpAtomic RpAtomic;

/**
 * \ingroup rpworlddatatypes
 * \typedef RpInterpolator
 * Morph Target Interpolator.
 * This should be considered an opaque type.
 * Use the RpInterpolator API functions to access.
 */
typedef struct RpInterpolator RpInterpolator;

#if (!defined(DOXYGEN))
struct RpInterpolator
{
        int             flags; /**< flags */
        short             startMorphTarget; /**< startMorphTarget */
        short             endMorphTarget; /**< endMorphTarget */
        float              time; /**< time */
        float              recipTime; /**< recipTime */
        float              position; /**< position */
};
#endif /* (!defined(DOXYGEN)) */

/* More callbacks */

/**
 * \ingroup rpworlddatatypes
 * \typedef RpClump
 * Clump Geometry object. This should be
 * considered an opaque type. Use the RpClump API functions to access.
 */
typedef struct RpClump RpClump;

/**
 * \ingroup rpworlddatatypes
 * \typedef RpClumpCallBack
 * \ref RpClumpCallBack represents the
 * function called from \ref RwCameraForAllClumpsInFrustum and
 * \ref RwCameraForAllClumpsNotInFrustum when a clump lies inside the current camera's
 * view frustum.  It is also called from \ref RpWorldForAllClumps.  This function should
 * return a pointer to the current clump to indicate success.  The callback may return
 * NULL to terminate further callbacks on the clumps.
 *
 * \param  clump   Pointer to the current clump, supplied by
 * iterator.
 * \param  data  Pointer to developer-defined data structure.
 *
 * \return Pointer to the current clump.
 *
 * \see RpWorldForAllClumps
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 * \see RwCameraForAllClumpsInFrustum
 * \see RwCameraForAllClumpsNotInFrustum
 */
typedef RpClump    *(*RpClumpCallBack) (RpClump * clump, void *data);

#if (!defined(DOXYGEN))
struct RpClump
{
        RwObject            object;

        /* Information about all the Atomics */
        RwLinkList          atomicList;

        /* Lists of lights and cameras */
        RwLinkList          lightList;
        RwLinkList          cameraList;

        /* The clump in a world */
        RwLLLink            inWorldLink;

        /* Clump frustum callback */
        RpClumpCallBack     callback;
};
#endif /* (!defined(DOXYGEN)) */

typedef struct RwObjectHasFrame RwObjectHasFrame;
typedef RwObjectHasFrame * (*RwObjectHasFrameSyncFunction)(RwObjectHasFrame *object);
struct RwObjectHasFrame
{
    RwObject                     object;
    RwLLLink                     lFrame;
    RwObjectHasFrameSyncFunction sync;
};


/**
 * \ingroup datatypes
 * \typedef RwResEntry 
 * RwResEntry object. Instanced data block in resources arena.  
 * This should be considered an opaque
 * type. Use the RwResEntry API functions to access.
 */
typedef struct RwResEntry RwResEntry;

/**
 * \ingroup datatypes
 * \typedef RwResEntryDestroyNotify
 * This type represents the function
 * called from \ref RwResourcesFreeResEntry (and indirectly from 
 * \ref RwResourcesEmptyArena) immediately before the memory used by the
 * specified resources entry is released.
 *
 * \param  entry   Pointer to the instanced data. 
 */
typedef void        (*RwResEntryDestroyNotify) (RwResEntry * resEntry);

#if (!defined(DOXYGEN))
struct RwResEntry
{
    RwLLLink            link;   /**< Node in the list of resource elements */
    int             size;   /**< Size of this node */
    void               *owner;  /**< Owner of this node */
    RwResEntry        **ownerRef; /**< Pointer to pointer to this (enables de-alloc) */
    RwResEntryDestroyNotify destroyNotify; /**< This is called right before destruction */
};
#endif /* (!defined(DOXYGEN)) */

/* Type ID */
#define rpGEOMETRY 8

/**
 * \ingroup rpworlddatatypes
 * RpGeometryFlag
 * Geometry type flags 
 */
enum RpGeometryFlag
{
    rpGEOMETRYTRISTRIP = 0x01,  /**<This geometry's meshes can be rendered
                                 * as strips */
    rpGEOMETRYPOSITIONS = 0x02, /**<This geometry has positions */  
    rpGEOMETRYTEXTURED = 0x04,  /**<This geometry has one set of texture coordinates */
    rpGEOMETRYPRELIT = 0x08,    /**<This geometry has luminance values */
    rpGEOMETRYNORMALS = 0x10,   /**<This geometry has normals */
    rpGEOMETRYLIGHT = 0x20,     /**<This geometry will be lit */
    rpGEOMETRYMODULATEMATERIALCOLOR = 0x40, /**<Modulate material color 
                                               with vertex colors (pre-lit + 
                                               lit) 
                                            */
    rpGEOMETRYTEXTURED2 = 0x80, /**<This geometry has 2 set of texture coordinates */

    /*
     * These flags are stored in the flags field in an RwObject, the flag field is 8bit.
     */

    rpGEOMETRYFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpGeometryFlag RpGeometryFlag;

/**
 * \ingroup rpworlddatatypes
 * RpGeometryLockMode
 * Geometry lock flags 
 */
enum RpGeometryLockMode
{
    rpGEOMETRYLOCKPOLYGONS = 1, /**<Lock the polygons */
    rpGEOMETRYLOCKVERTICES = 2, /**<Lock the vertices */
    rpGEOMETRYLOCKNORMALS = 4,  /**<Lock the normals */
    rpGEOMETRYLOCKTEXCOORDS = 8, /**<Lock the texture coordinates set 1*/
    rpGEOMETRYLOCKTEXCOORDS2 = 16, /**<Lock the texture coordinates set 2*/
    rpGEOMETRYLOCKPRELIGHT = 32, /**<Lock the pre-light values */
    rpGEOMETRYLOCKALL = 63,      /**<Combination of all the above */
    rpGEOMETRYLOCKMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpGeometryLockMode RpGeometryLockMode;

enum _rpGeometryInstanceFlags
{
    rpGEOMETRYPERSISTENT = 0x01,
    rpGEOMETRYINSTANCE   = 0x02,
    rpGEOMETRYINSTANCEFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum _rpGeometryInstanceFlags rpGeometryInstanceFlags;

typedef struct RwSphere RwSphere;

struct RwSphere
{
    CVector center;   /**< Sphere center */
    float radius;  /**< Sphere radius */
};


/**
 * \ingroup rpworlddatatypes
 * \typedef RpGeometry 
 * Geometry object. This should be considered an opaque type. 
 * Use the RpGeometry API functions to access.
 */
typedef struct RpGeometry RpGeometry;

/**
 * \ingroup rpworlddatatypes
 * \typedef RpMorphTarget 
 * Morph target -- vertex positions and normals. 
 * This should be considered an opaque type. 
 * Use RpMorphTarget API functions to access.
 */
typedef struct RpMorphTarget RpMorphTarget;

#if (!defined(DOXYGEN))
struct RpMorphTarget
{
        RpGeometry *parentGeom; 
        RwSphere   boundingSphere;
        CVector      *verts;
        CVector      *normals;
};
#endif /* (!defined(DOXYGEN)) */

typedef struct RpTriangle RpTriangle;

/**
 * \ingroup rpworlddatatypes
 * \struct RpTriangle 
 * This type represents a triangle in a geometry specified 
 * by three indices into the geometry's vertex list (vertIndex)
 * together with an  index in to the geometry's 
 * material list (matIndex) (see API function \ref RpGeometryGetTriangles)
 */
struct RpTriangle
{
    uint16_t            vertIndex[3]; /**< vertex indices */
    short             matIndex; /**< Index into material list */
};

/**
 * \ingroup rpworlddatatypes
 * \typedef RpMaterial 
 * Material object. This should be 
 * considered an opaque type. Use the RpMaterial API functions to access.
 */
typedef struct RpMaterial RpMaterial;

typedef struct RwSurfaceProperties RwSurfaceProperties;
/**
 * \ingroup datatypes
 * \struct RwSurfaceProperties
 *  This type represents the ambient, diffuse and
 * specular reflection coefficients of a particular geometry. Each coefficient
 * is specified in the range 0.0 (no reflection) to 1.0 (maximum reflection).
 */
struct RwSurfaceProperties
{
    float ambient;   /**< ambient reflection coefficient */
    float specular;  /**< specular reflection coefficient */
    float diffuse;   /**< reflection coefficient */
};

#define RXPIPELINE 1

#if (!defined(DOXYGEN))
struct RpMaterial
{
        RwTexture           *texture; /**< texture */
        RwRGBA              color; /**< color */              
#if (defined(RXPIPELINE))
        void          *pipeline; /**< pipeline */     

        RwSurfaceProperties surfaceProps; /**< surfaceProps */
#else /* (defined(RXPIPELINE)) */
        /* Not used by the Pipeline1 rendering
         * but present so pp files can be loaded and
         * saved and the values not destroy
         */
        RwSurfaceProperties ignoredSurfaceProps;    

#endif /* (defined(RXPIPELINE)) */

        short             refCount;          /* C.f. rwsdk/world/bageomet.h:RpGeometry */
        short             pad;
};
#endif /* (!defined(DOXYGEN)) */

typedef struct RpMaterialList RpMaterialList;
struct RpMaterialList
{
    RpMaterial     **materials;
    RwInt32        numMaterials;
    RwInt32        space;
};

typedef struct RwTexCoords RwTexCoords;

struct RwTexCoords
{
    float u;   /**< U value */
    float v;   /**< V value */
};

typedef struct RpMeshHeader RpMeshHeader;

/**
 * \ingroup rpworlddatatypes
 * \ref RpMeshHeaderFlags
 * represents the different types of mesh.
 * \see RpMeshHeader
 */
enum RpMeshHeaderFlags
{
    /* NOTE: trifans are denoted by absence of any other primtype
     *       flags so be careful that you test:
     *        (flags&triListFlag == triListFlag)
     *       and not:
     *        (flags&triListFlag)
     */
    rpMESHHEADERTRISTRIP = 0x0001,  /**< Render as tristrips */
    rpMESHHEADERTRIFAN = 0x0002,    /**< On PS2 these will be converted to trilists */
    rpMESHHEADERLINELIST = 0x0004,  /**< Render as linelists */
    rpMESHHEADERPOLYLINE = 0x0008,  /**< On PS2 these will be converted to linelists */
    rpMESHHEADERPOINTLIST = 0x0010, /**< Pointlists are supported only if rendered by
                                     *   custom pipelines; there is no default RenderWare 
                                     *   way to render pointlists. */

    rpMESHHEADERPRIMMASK = 0x00FF,  /**< All bits reserved for specifying primitive type */
    rpMESHHEADERUNINDEXED = 0x0100, /**< Topology is defined implicitly by vertex
                                     *   order, ergo the mesh contains no indices */
    rpMESHHEADERFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*
 * Typedef for RpMeshHeaderFlags enumeration
 * representing the different types of mesh
 */
typedef enum RpMeshHeaderFlags RpMeshHeaderFlags;


typedef struct RpBuildMeshTriangle RpBuildMeshTriangle;

/**
 * \ingroup rpworlddatatypes
 * \struct RpBuildMeshTriangle
 * This type represents an array of indices into
 * the object vertex array. Used during the construction
 * of tristrips.
 *
 * See API functions
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * and
 * \see RpMeshSetTriStripMethod
 * \see RpMeshGetTriStripMethod
 */
struct RpBuildMeshTriangle
{
    uint16_t            vertIndex[3]; /**< indices into object vertex
                                       *   array. */
    RpMaterial         *material;     /**< pointer to material used to
                                       *   render the mesh. */
};

typedef struct RpBuildMesh RpBuildMesh;

/**
 * \ingroup rpworlddatatypes
 * \struct RpBuildMesh
 * This type represents a mesh ready for tri stripping.
 *
 * See API functions
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * and
 * \see RpMeshSetTriStripMethod
 * \see RpMeshGetTriStripMethod
 */
struct RpBuildMesh
{
    RwUInt32            triangleBufferSize; /**< number of triangles
                                             *   space has been allocated
                                             *   for. */
    RwUInt32            numTriangles;       /**< number of triangles to be
                                             *   tristripped. */
    RpBuildMeshTriangle *meshTriangles;     /**< pointer to build mesh
                                             *   triangles. */
};

typedef struct RpMesh RpMesh;

/**
 * \ingroup rpworlddatatypes
 * \struct RpMesh
 * This type represents a single polygon mesh.
 * A mesh is defined as a collection of triangles derived from an RpGeometry
 * or RpWorldSector which have a common material.
 *
 * See API functions \see RpGeometryForAllMeshes and
 * \see RpWorldSectorForAllMeshes and
 * the corresponding function callback types:
 */
typedef uint16_t        RxVertexIndex;
typedef RxVertexIndex   RwImVertexIndex;

struct RpMesh
{
    RxVertexIndex      *indices;    /**< vertex indices defining the mesh */
    RwUInt32            numIndices; /**< number of vertices in mesh */
    RpMaterial         *material;   /**< pointer to material used to
                                     *   render the mesh. */
};

/**
 * \ingroup rpworlddatatypes
 * \struct RpMeshHeader
 * Header for all meshes that constitute a single RpGeometry or RpWorldSector
 */
struct RpMeshHeader
{
    RwUInt32            flags;    /**< \see RpMeshHeaderFlags */
    uint16_t            numMeshes; /**< Number of meshes in object */
    uint16_t            serialNum; /**< Determine if mesh has changed
                                    * since last instance */
    RwUInt32            totalIndicesInMesh; /**< Total triangle index
                                             * count in all meshes
                                             */
    RwUInt32            firstMeshOffset; /**< Offset in bytes from end this
                                          * structure RpMeshHeader
                                          * to the first mesh
                                          */
};

#define rwMAXTEXTURECOORDS 8

#if (!defined(DOXYGEN))
struct RpGeometry
{
    RwObject            object;     /* Generic type */

    RwUInt32            flags;      /* Geometry flags */

    uint16_t            lockedSinceLastInst; /* What has been locked since we last instanced - for re-instancing */
    short             refCount;   /* Reference count (for keeping track of atomics referencing geometry) */

    RwInt32             numTriangles; /* Quantity of various things (polys, verts and morph targets) */
    RwInt32             numVertices;
    RwInt32             numMorphTargets;
    RwInt32             numTexCoordSets;

    RpMaterialList      matList;

    RpTriangle         *triangles;  /* The triangles */

    RwRGBA             *preLitLum;  /* The pre-lighting values */

    RwTexCoords        *texCoords[rwMAXTEXTURECOORDS]; /* Texture coordinates */

    RpMeshHeader       *mesh;   /* The mesh - groups polys of the same material */

    RwResEntry         *repEntry;       /* Information for an instance */

    RpMorphTarget      *morphTarget;    /* The Morph Target */
};
#endif /* (!defined(DOXYGEN)) */


/**
 * \ingroup rpworlddatatypes
 * \typedef RpAtomicCallBackRender
 * \ref RpAtomicCallBackRender represents the function called from
 * \ref RpAtomicRender when the specified atomic
 * lies inside the current camera's view frustum. The default callback
 * initiates execution of the atomic rendering pipeline. This function should
 * return a pointer to the atomic to indicate success.
 *
 * \param  atomic   Pointer to the current atomic, supplied by
 * iterator.
 *
 * \see RpAtomicRender
 */
typedef RpAtomic   *(*RpAtomicCallBackRender) (RpAtomic * atomic);

#if (!defined(DOXYGEN))
struct RpAtomic
{
    RwObjectHasFrame    object;

    /* Information for an instance */
    RwResEntry         *repEntry;

    /* Triangles making the object */
    RpGeometry         *geometry;

    /* Interpolated bounding sphere (in object space and world space) */
    RwSphere            boundingSphere;
    RwSphere            worldBoundingSphere;

    /* Connections to other atomics */
    RpClump            *clump;
    RwLLLink            inClumpLink;

    /* callbacks */
    RpAtomicCallBackRender renderCallBack;

    /* Interpolation animation pointer */
    RpInterpolator      interpolator;

    /* Counter for checks of collision/render has occured already */
    uint16_t            renderFrame;
    uint16_t            collisionFrame;

    /* Connections to sectors */
    RwLinkList          llWorldSectorsInAtomic;

#ifdef RXPIPELINE
    /* The Atomic instancing pipeline for this Atomic */
    void         *pipeline;
#endif
};
#endif /* (!defined(DOXYGEN)) */


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
    float         camVertex_z;            /**< Camera space z (for fog table) */
    float         u;                      /**< Texture coordinate */
    float         v;                      /**< Texture coordinate */
    float         recipZ;                 /**< Recipz */
    float         pad1;                   /**< Padding (fog table) */
    RwRGBAReal     color;                  /**< Color */
    CVector        objNormal;              /**< We allocate this, even though
                                            * we don't use it - we may want
                                            * to later.
                                            */
    float         pad2;                   /**< Alignment padding */
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
         *  The value should be a pointer to an float in the 
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


/**
 * \ingroup datatypes
 * \ref RwRasterFormat is a set of values and flags which may be combined to
 * specify a raster format. The format chosen for a particular raster depends
 * on the hardware device and the raster type specified at creation time
 * (see API function \ref RwRasterCreate). The format may be retrieved using
 * API function \ref RwRasterGetFormat.
 *
 * The raster format is a packed set of bits which contains the following
 * four pieces of information (these may be combined with bitwise OR):-
 *
 * <ol>
 * <li> The pixel color format corresponding to one of the following values:-
 *      <ul>
 *      <li> rwRASTERFORMAT1555
 *      <li> rwRASTERFORMAT565
 *      <li> rwRASTERFORMAT4444
 *      <li> rwRASTERFORMATLUM8
 *      <li> rwRASTERFORMAT8888
 *      <li> rwRASTERFORMAT888
 *      <li> rwRASTERFORMAT16
 *      <li> rwRASTERFORMAT24
 *      <li> rwRASTERFORMAT32
 *      <li> rwRASTERFORMAT555
 *      </ul>
 *      This value may be masked out of the raster format using
 *      rwRASTERFORMATPIXELFORMATMASK.
 * <li> The palette depth if the raster is palettized:-
 *      <ul> 
 *      <li> rwRASTERFORMATPAL4
 *      <li> rwRASTERFORMATPAL8
 *      </ul>
 *      In these cases, the color format refers to that of the palette.
 * <li> Flag rwRASTERFORMATMIPMAP. Set if the raster contains mipmap levels.
 * <li> Flag rwRASTERFORMATAUTOMIPMAP. Set if the mipmap levels were generated
 *      automatically by RenderWare.
 * </ol>
 */
enum RwRasterFormat
{
    rwRASTERFORMATDEFAULT = 0x0000, /* Whatever the hardware likes best */

    rwRASTERFORMAT1555 = 0x0100,    /**<16 bits - 1 bit alpha, 5 bits red, green and blue */
    rwRASTERFORMAT565 = 0x0200,     /**<16 bits - 5 bits red and blue, 6 bits green */
    rwRASTERFORMAT4444 = 0x0300,    /**<16 bits - 4 bits per component */
    rwRASTERFORMATLUM8 = 0x0400,    /**<Gray scale */
    rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
    rwRASTERFORMAT888 = 0x0600,     /**<24 bits - 8 bits per component */
    rwRASTERFORMAT16 = 0x0700,      /**<16 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT24 = 0x0800,      /**<24 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT32 = 0x0900,      /**<32 bits - undefined: useful for things like Z buffers */
    rwRASTERFORMAT555 = 0x0a00,     /**<16 bits - 5 bits red, green and blue */

    rwRASTERFORMATAUTOMIPMAP = 0x1000, /**<RenderWare generated the mip levels */

    rwRASTERFORMATPAL8 = 0x2000,    /**<8 bit palettised */
    rwRASTERFORMATPAL4 = 0x4000,    /**<4 bit palettised */

    rwRASTERFORMATMIPMAP = 0x8000,  /**<Mip mapping on */

    rwRASTERFORMATPIXELFORMATMASK = 0x0f00, /**<The pixel color format 
                                             *  (excluding palettised bits) */
    rwRASTERFORMATMASK = 0xff00     /**<The whole format */ ,
    rwRASTERFORMATFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwRasterFormat RwRasterFormat;

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

enum RwRasterLockMode
{
    rwRASTERLOCKWRITE = 0x01,   /**<Lock for writing */
    rwRASTERLOCKREAD = 0x02,    /**<Lock for reading */
    rwRASTERLOCKNOFETCH = 0x04, /**<When used in combination with
                                 *  rwRASTERLOCKWRITE, asks the driver not to
                                 *  fetch the pixel data. This is only useful
                                 *  if it is known that ALL the raster data is
                                 *  going to be overwritten before the raster
                                 *  is unlocked, i.e. from an 
                                 *  \ref RwRasterSetFromImage call. This flag
                                 *  is not supported by all drivers. */
    rwRASTERLOCKRAW = 0x08,    /**<When used in combination with
                                   rwRASTERLOCKWRITE or rwRASTERLOCKREAD
                                   allows access to the raw platform specific
                                   pixel format */
    rwRASTERLOCKMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

extern float RecipNearClip;
extern float NearScreenZ;

void psCameraShowRaster(void*);
void RwIm2DRenderPrimitive(RwInt32, RwIm2DVertex*, RwInt32);
int RwRenderStateSet(RwInt32, void*);
void RwIm3DRenderIndexedPrimitive(int, RwImVertexIndex*, RwInt32);
void* RwIm3DTransform(RxObjSpace3dVertex *pVerts, RwUInt32 numVerts, RwMatrix *ltm, RwUInt32 flags);
void RwIm3DEnd();
void RwIm2DRenderIndexedPrimitive(int, RwIm2DVertex*, RwInt32, RwImVertexIndex*, RwInt32);
RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
int RwRasterDestroy(RwRaster * raster);
RwRaster* RwRasterSubRaster(RwRaster* subRaster, RwRaster* raster, RwRect* rect);
RwUInt8* RwRasterLock(RwRaster * raster, RwUInt8 level, RwInt32 lockMode);
RwUInt8* RwRasterLockPalette(RwRaster * raster, RwInt32 lockMode);
RwRaster* RwRasterUnlock(RwRaster * raster);
RwTexture* RwTextureCreate(RwRaster * raster);
int RwTextureDestroy(RwTexture * texture);
void RpSkyTexCacheRasterLock(RwRaster *raster, int bLocked);
void RwRasterPushContext(RwRaster* raster);
void RwRasterPopContext();
RwRaster* RwRasterRenderFast(RwRaster * raster, RwInt32 x, RwInt32 y);
RwRaster* RwRasterRender(RwRaster * raster, RwInt32 x, RwInt32 y);
RwRaster* RwRasterRenderScaled(RwRaster * raster, RwRect * rect);
void* RwCameraBeginUpdate(void * camera);
void* RwCameraEndUpdate(void * camera);
void* RwCameraClear(void *camera, RwRGBA *colour, RwInt32 clearMode);
RwRaster* RpSkyTexSetTex0(RwRaster *r, RwUInt32 msb, RwUInt32 lsb);
RwRaster* RpSkyTexGetTex0(RwRaster *r, RwUInt32 *msb, RwUInt32 *lsb);
RwRaster* RpSkyGetDisplayBufferRaster(void);
RwRaster* RpSkyGetDrawBufferRaster(void);


void *RwStreamOpen(uint32_t type, uint32_t accessType, const void *pData);
bool RwStreamClose(void * stream, void *pData);
uint32_t RwStreamRead(void * stream, void *buffer, uint32_t length);
bool RwStreamFindChunk(void *stream, uint32_t type, uint32_t *lengthOut, uint32_t *versionOut);
RpClump *RpClumpStreamRead(void * stream);
RpAtomic *ClumpGetFirstAtomic(void *clump);
void *AtomicDefaultRenderCallBack(void * atomic);


typedef struct RpHAnimHierarchy RpHAnimHierarchy;

struct RpHAnimHierarchy
{
    int             flags;          /**< Flags for the hierarchy  */
    int             numNodes;      /**< Number of nodes in the hierarchy  */

    RwMatrix           *pMatrixArray;   /**< Pointer to node matrices*/
    void               *pMatrixArrayUnaligned; /**< Pointer to memory used for node matrices
                                                * from which the aligned pMatrixArray is allocated */
    void    *pNodeInfo;     /**< Array of node information (push/pop flags etc) */
    RwFrame            *parentFrame;    /**< Pointer to the Root RwFrame of the hierarchy this
                                           * RpHAnimHierarchy represents */
    RpHAnimHierarchy   *parentHierarchy;                       /**< Internal use */
    int             rootParentOffset;                      /**< Internal use */

    void   *currentAnim;                    /**< Internal use */
};


void *RwFrameRotate(RwFrame * frame, CVector * axis, float angle, int combine);
void *RwFrameScale(RwFrame * frame, CVector * v, int combine);
CVector *RwV3dTransformPoints(CVector *pointsOut, CVector *pointsIn, int numPoints, RwMatrix *matrix);
void* RwFrameUpdateObjects(RwFrame *frame);
void* RpClumpRender(RpClump * clump);
RpHAnimHierarchy *GetAnimHierarchyFromSkinClump(RpClump *clump);
RwMatrix* RpHAnimHierarchyGetMatrixArray(RpHAnimHierarchy *a1);
int RpHAnimIDGetIndex(void **a1, int id);

RwFrame *RwFrameTranslate(RwFrame * frame, CVector * v, int combine);
RwFrame *RwFrameSetIdentity(RwFrame * frame);

RwMatrix *RwMatrixTranslate(RwMatrix * matrix, CVector * translation, int combineOp);

RwMatrix *RwMatrixRotate(RwMatrix * matrix, CVector * axis, float angle, int combineOp);

RwMatrix *RwMatrixUpdate(RwMatrix * matrix);

RwFrame *RwFrameTransform(RwFrame *frame, RwMatrix *transform, int combineOp);

bool RpClumpDestroy(RpClump * clump);


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

RwFileFunction* RwOsGetFileInterface(void);

#   define MAKE128(RES, MSB, LSB) \
    __asm volatile ( "pcpyld %0, %1, %2" : "=r" (RES) : "r" (MSB), "r" (LSB))

#define _rwDMAPktPtr (*(__uint128_t**)0x66C170)

int _rwDMAOpenGIFPkt(RwUInt32, RwUInt32);
int _rwDMAOpenVIFPkt(RwUInt32, RwUInt32);

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


extern uint32_t skyFrameBit;
extern bool skySuspended;

extern void _rwDMAFlipData;

extern void skyInitialClear;

extern uint32_t currentMode;
int sceGsSyncV(int mode);
void sceGsResetGraph(short mode, short inter, short omode, short ffmode);
int sceGsSwapDBuffDc(void *db, int id);
int sceGsSyncPath(int mode, uint16_t timeout);
void *sceDmaGetChan(int id);
void sceDmaSendN(void *d, void *addr, int size);

void setInitialRenderState(void);

void RpSkyTexCacheFlush(void);

bool _rwDMAHook();

RwInt32 RwEngineGetNumVideoModes(void);
RwVideoMode *RwEngineGetVideoModeInfo(RwVideoMode *modeinfo, RwInt32 modeIndex);
RwInt32 RwEngineGetCurrentVideoMode(void);
bool RwEngineSetVideoMode(RwInt32 modeIndex);

void RsRwInitialize(void*);
void RsRwTerminate(void);

void SkyStart(void);

void RpSkySuspend(void);

void sceGsSetDefDBuffDc(void *db, short psm, short w, short h, short ztest, short zpsm, short clear);

#endif
