#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CRGBA.h>
#include <CVector.h>
#include <CVehicle.h>
#include <CTexture.h>
#include <CTxdStore.h>
#include <common.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCam.h>
#include <math.h>

#include "modules.h"
#include "renderbuffer.h"
#include "hooks.h"

static void (*CGame_Init2)(const char*);

static void* Camera = (void*)0x66C2FC;

static void (*hook_RenderEffects)();

static int (*CCustomCarEnvMapPipeline__SetFxEnvTexture)(void *a1, RwTexture *a2) = (int (*)(void *a1, RwTexture *a2))0x507B90;
static int (*CCustomCarEnvMapPipeline__SetFxSpecTexture)(void *a1, RwTexture *a2) = (int (*)(void *a1, RwTexture *a2))0x507D00;

static RwRaster *pEnvRaster, *pSpecRaster;
static RwTexture *ScreenSpaceEnvTexture, *ScreenSpaceSpecTexture;

void SetSpecReflections(void *a1, RwTexture *a2) {
    CCustomCarEnvMapPipeline__SetFxSpecTexture(a1, ScreenSpaceSpecTexture);
}

void SetEnvReflections(void *a1, RwTexture *a2) {
    CCustomCarEnvMapPipeline__SetFxEnvTexture(a1, ScreenSpaceEnvTexture);
}

void CreateScreenSpaceTexture(const char* a1) {
    CGame_Init2(a1);

    pEnvRaster = RwRasterCreate(128, 128, 24, rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE);

    RpSkyTexCacheRasterLock(pEnvRaster, true);
    
    ScreenSpaceEnvTexture = RwTextureCreate(pEnvRaster);
    ScreenSpaceEnvTexture->filterAddressing = rwFILTERLINEAR;

    pSpecRaster = RwRasterCreate(128, 128, 24, rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE);

    RpSkyTexCacheRasterLock(pSpecRaster, true);

    ScreenSpaceSpecTexture = RwTextureCreate(pSpecRaster);
    ScreenSpaceSpecTexture->filterAddressing = rwFILTERLINEAR;
}

void RenderScreenSpaceEnvTexture() {
    GetBackBufferOffset(pEnvRaster, 32, 128);
    GetBackBufferOffset(pSpecRaster, 256, 0);

    hook_RenderEffects();
}

/*RpSkyResume(void)
{
    if (!*skySuspended)
    {
       return false;
    }

    sceGsSyncV(0);

    sceGsResetGraph(0, GS_NOINTERLACE, GS_NTSC, GS_FRAME);

    sceGsSwapDBuffDc(_rwDMAFlipData, *skyFrameBit);
    *skyFrameBit = *skyFrameBit + 1;

    sceDmaSendN(sceDmaGetChan(2), skyInitialClear, 13);

    RpSkyTexCacheFlush();
    setInitialRenderState();
    *skySuspended = !_rwDMAHook();
    return *skySuspended;
}*/


static RwTexture *texWaterclear256 = (RwTexture *)0x66B380;

void (*CWaterLevel_RenderWater)(void) = (void (*)(void))0x182850;

uint32_t time_to_rotate = 0;

void swap(int *xp, int *yp) {
    int tmp = *xp;
    *xp = *yp;
    *yp = tmp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    int j;
    for (j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void r_quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        r_quickSort(arr, low, pi - 1);
        r_quickSort(arr, pi + 1, high);
    }
}

void quickSort(int arr[], int n) {
    r_quickSort(arr, 0, n-1); // It's still recursive, but I've normalized the function call so it can be placed in arrays or structs
}


uint8_t pack4bit(uint8_t pixel1, uint8_t pixel2) {
    return (pixel1 << 4) | (pixel2 & 0x0F);
}

void unpack4bit(uint8_t packedByte, uint8_t *pixel1, uint8_t *pixel2) {
    *pixel1 = (packedByte >> 4) & 0x0F;
    *pixel2 = packedByte & 0x0F;
}


void sort_water_texture() {
    RwRGBA *in_pallete = (RwRGBA *)texWaterclear256->raster->palette;
    uint8_t *in_texture = texWaterclear256->raster->cpPixels;
    RwRGBA out_palette[16];

    int i;
    int j;
    int k;

    memcpy(out_palette, in_pallete, 16*sizeof(RwRGBA));

    quickSort((int*)out_palette, 16);

    uint8_t pixels[2];
    for (i = 0; i < (texWaterclear256->raster->width*(texWaterclear256->raster->height/2)); i++) {
        unpack4bit(in_texture[i], &pixels[0], &pixels[1]);

        for (j = 0; j < 2; j++) {
            for (k = 0; k < 16; k++) {
                if (((int*)out_palette)[k] == ((int*)in_pallete)[pixels[j]]) {
                    pixels[j] = k;
                    break;
                }
            }
        }

        in_texture[i] = pack4bit(pixels[0], pixels[1]);
    }

    memcpy(in_pallete, out_palette, 16*sizeof(RwRGBA));

    CTxdStore_PopCurrentTxd();
}

void rotate_up(RwRGBA *out, RwRGBA *in) {
    memcpy(&out[0], &in[14], sizeof(RwRGBA));
    memcpy(&out[1], &in[0], 14*sizeof(RwRGBA));
    memcpy(in, out, 15*sizeof(RwRGBA));
}

void rotate_down(RwRGBA *out, RwRGBA *in) {
    memcpy(&out[14], &in[0], sizeof(RwRGBA));
    memcpy(&out[0], &in[1], 14*sizeof(RwRGBA));
    memcpy(in, out, 15*sizeof(RwRGBA));
}

typedef enum {
    UP,
    DOWN
} ShiftDirection;

static int shift_count = 0;
static int hold_count = 0;
static ShiftDirection direction = UP;
static bool hold = false;
static int hold_coef = 0;

void rotate_clut() {
    RwRGBA out_pallete[16];
    RwRGBA *in_pallete = (RwRGBA *)RwRasterLockPalette(texWaterclear256->raster, 1);

    if (CTimer_m_snTimeInMilliseconds > time_to_rotate) {
        if (!hold) {
            if (direction == UP) {
                rotate_up(out_pallete, in_pallete);
            } else {
                rotate_down(out_pallete, in_pallete);
            }

            shift_count++;
        } else {
            if (CTimer_m_snTimeInMilliseconds > hold_coef) {
                hold = false;
            }
        }

        if (shift_count > 14) {
            direction ^= 1;
            shift_count = 0;
            hold = true;
            hold_coef = CTimer_m_snTimeInMilliseconds + (35*randint(1, 7));
        }

        time_to_rotate = CTimer_m_snTimeInMilliseconds + (35*randint(1, 3));
    }

    CWaterLevel_RenderWater();
}

extern int skyRasterExt;

extern uint64_t skyFrame_1;
extern uint64_t skyZbuf_1;

typedef struct _SkyMemBlock _SkyMemBlock;
struct _SkyMemBlock
{
    RwRaster *raster;  /**< Internal use */
    uint32_t address;  /**< Internal use */
    uint32_t size;     /**< Internal use */
};

typedef struct _SkyRasterExt _SkyRasterExt;
struct _SkyRasterExt
{
    int           dmaRefCount;                      /**< Internal use */
    int           dmaClrCount;                      /**< Internal use */

    /* General texture setup register */
    uint32_t          lsb;                              /**< Internal use */
    uint32_t          msb;                              /**< Internal use */
    uint32_t          palOffset;                        /**< Internal use */

    /* K: a 12 bit 8.4 value in bottom bits */
    /* L: a 2 bit value in 12,13 */
    uint16_t          mipmapKL;                         /**< Internal use */
    /* NOTE: This is left shifted two */
    uint8_t           maxMipLevel;                      /**< Internal use */
    /* Is this texture to stay in the cache? */
    uint8_t           bLocked;                          /**< Internal use */

    /* Mipmap addresses */
    uint32_t          miptbp1Lsb;                       /**< Internal use */
    uint32_t          miptbp1Msb;                       /**< Internal use */
    uint32_t          miptbp2Lsb;                       /**< Internal use */
    uint32_t          miptbp2Msb;                       /**< Internal use */

    /* Size in bytes in system memory for pixels */
    uint32_t          sysMemSize;                       /**< Internal use */
    /* Size in bytes in system memory for palette */
    uint32_t          sysMemPalSize;                    /**< Internal use */

    /* Size in words in video memory for pixels + palette */
    uint32_t          nTexCacheSize;                    /**< Internal use */

    /* Should we cache packets for this raster */
    uint8_t           cachePkts;                        /**< Internal use */
    uint8_t           lockedMipLevel;     /**< Currently locked mip level */
    uint8_t           flags;                /**< Bit 0 new format texture */
                                               /**< Bit 1 twiddled (->32) */
                                              /**< Bit 2 twiddled (->16)  */
    uint8_t           pad[1];                           /**< Internal use */
    /* Cached packet for uploading the palette */
    __uint128_t         *palUploadPkt;                    /**< Internal use */
    /* Cached packets for uploading the mip levels */
    __uint128_t         *mipUploadPkts[7];                /**< Internal use */

    /* The memory block that currently holds this texture */
    _SkyMemBlock          *mpCacheEntry;                /**< Internal use */
};

#define RASTEREXTFROMRASTER(raster) \
    ((_SkyRasterExt *)(((uint8_t *)(raster)) + skyRasterExt))


#define GIF_PRE_ENABLE  0x01

void _rwDMAWaitQueue();
/*
bool SkyEdgeCameraClear(uint8_t *pCam, RwRGBA *col, int nClearWhich)
{
    uint64_t tmp, tmp1;
    RwRaster* camRaster = *(RwRaster**)(pCam + 96);

    _rwDMAWaitQueue();

    if ((camRaster->cType & rwRASTERTYPEMASK) == rwRASTERTYPECAMERATEXTURE)
    {
        _SkyRasterExt *reExt = RASTEREXTFROMRASTER(camRaster);

        // If the texture is not locked in cache we fail 
        if (!reExt->bLocked)
        {
            //RWMESSAGE((RWSTRING("error - rwRASTERTYPECAMERATEXTURE must be locked in cache")));
            return false;
        }
        // We build a fake frame_1 to permit rendering 
        skyFrame_1 = ((reExt->lsb>>5)&0x1ff) | ((reExt->lsb<<2)&0x3f0000) | ((reExt->lsb<<4)&0x3f000000);
    }
    else
    {
        if (skyFrameBit & 0x1)
        {
            skyFrame_1 = *(uint64_t *)0x829900; // &_rwDMAFlipData.db.draw01.frame1
        }
        else
        {
            skyFrame_1 = *(uint64_t *)0x829A70; // &_rwDMAFlipData.db.draw11.frame1
        }
    }

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 5);

    tmp = GIF_SET_TAG(4, 1, GIF_PRE_ENABLE, GS_PRIM_SPRITE, GIF_PACKED, 1);
    tmp1 = GIF_AD;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_FRAME_1(skyFrame_1, 1, GS_PSMCT32, 0);
    tmp1 = GS_FRAME_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_ZBUF_1((skyFrame_1) + 70, GS_PSMZ32, 0);
    tmp1 = GS_ZBUF_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_TEST_1(0, 0, 0, 0, 0, 0, 1, 1);
    tmp1 = GS_TEST_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_SCISSOR_1(0, 64 - 1, 0, 2048 - 1);
    tmp1 = GS_SCISSOR_1;
    ADDTOPKT(tmp1, tmp);

    _rwDMAOpenVIFPkt(RWDMA_FIXUP, 11);
    
    // Clear
    tmp = GIF_SET_TAG(10, 1, GIF_PRE_ENABLE, GS_PRIM_SPRITE, GIF_PACKED, 1);
    tmp1 = GIF_AD;
    ADDTOPKT(tmp1, tmp);

    //---------------------------------------------------------//

    tmp = GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x01, 1);
    tmp1 = GS_RGBAQ;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_FRAME_1(skyZbuf_1, 1, GS_PSMCT32, 0);
    tmp1 = GS_FRAME_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_ZBUF_1(skyZbuf_1, GS_PSMZ32, 0);
    tmp1 = GS_ZBUF_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_XYZ2(0, 0, 0);
    tmp1 = GS_XYZ2;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_XYZ2((32 << 4), 2048 << 4, 0x00000000);
    tmp1 = GS_XYZ2;
    ADDTOPKT(tmp1, tmp);

   //---------------------------------------------------------//

    tmp = GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x01, 1);
    tmp1 = GS_RGBAQ;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_FRAME_1(skyZbuf_1 + 64, 1, GS_PSMCT32, 0);
    tmp1 = GS_FRAME_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_ZBUF_1(skyZbuf_1 + 64, GS_PSMZ32, 0);
    tmp1 = GS_ZBUF_1;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_XYZ2(0, 0, 0);
    tmp1 = GS_XYZ2;
    ADDTOPKT(tmp1, tmp);

    tmp = GS_SET_XYZ2((32 << 4), 192 << 4, 0x00000000);
    tmp1 = GS_XYZ2;
    ADDTOPKT(tmp1, tmp);

    return true;
}

*/

void ReturnShadowCrossProductOffset() {
    float test_cpoffset = -0.5f;
    asm volatile ("mtc1 %0, $f1" : : "r" (*(uint32_t*)&test_cpoffset));
}

void ReturnShadowCrossProductOffset0() {
    float test_cpoffset = -0.5f;
    asm volatile ("mtc1 %0, $f0" : : "r" (*(uint32_t*)&test_cpoffset));
}

void installSkyEdgeEngine()
{  
    InjectRubbishPatches();

    MakeNop(0x1190E4);

    RedirectCall(0x119358, ReturnShadowCrossProductOffset);
    MakeNop(0x11935C);

    RedirectCall(0x1198F8, ReturnShadowCrossProductOffset0);
    RedirectCall(0x11A0C0, ReturnShadowCrossProductOffset0);

    RedirectCall(0x152D88, SunGlareInject);
    RedirectCall(0x152D9C, SunGlareInject);
    RedirectCall(0x152DB4, SunGlareInject);

    CGame_Init2 = (void (*)())ReadCall(0x242BF4);

    hook_RenderEffects = (void (*)())ReadCall(0x246990);

    RedirectCall(0x246990, RenderScreenSpaceEnvTexture);

    RedirectCall(0x242BF4, CreateScreenSpaceTexture);
    //RedirectCall(0x246988, RenderScreenSpaceEnvTexture);
    RedirectFunction(0x3C8F00, SetEnvReflections);
    RedirectFunction(0x3C8F10, SetSpecReflections);

    //RpSkySuspend();
    //RpSkyResume();

    RedirectCall(0x246CC8, rotate_clut);
    RedirectCall(0x246E20, rotate_clut);
    RedirectCall(0x17F8A8, sort_water_texture);

    //RedirectCall(0x24693C, SkyEdgeCameraClear);
}
