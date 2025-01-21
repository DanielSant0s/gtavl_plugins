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

#include "model_ids.h"
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

static void SetSpecReflections(void *a1, RwTexture *a2) {
    CCustomCarEnvMapPipeline__SetFxSpecTexture(a1, ScreenSpaceSpecTexture);
}

static void SetEnvReflections(void *a1, RwTexture *a2) {
    CCustomCarEnvMapPipeline__SetFxEnvTexture(a1, ScreenSpaceEnvTexture);
}

static void CreateScreenSpaceTexture(const char* a1) {
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

static void RenderScreenSpaceEnvTexture() {
    seGetBackBufferOffset(pEnvRaster, 32, 128);
    seGetBackBufferOffset(pSpecRaster, 256, 0);

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

static uint32_t time_to_rotate = 0;

static void swap(int *xp, int *yp) {
    int tmp = *xp;
    *xp = *yp;
    *yp = tmp;
}

static int partition(int arr[], int low, int high) {
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

static void r_quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        r_quickSort(arr, low, pi - 1);
        r_quickSort(arr, pi + 1, high);
    }
}

static void quickSort(int arr[], int n) {
    r_quickSort(arr, 0, n-1); // It's still recursive, but I've normalized the function call so it can be placed in arrays or structs
}


static uint8_t pack4bit(uint8_t pixel1, uint8_t pixel2) {
    return (pixel1 << 4) | (pixel2 & 0x0F);
}

static void unpack4bit(uint8_t packedByte, uint8_t *pixel1, uint8_t *pixel2) {
    *pixel1 = (packedByte >> 4) & 0x0F;
    *pixel2 = packedByte & 0x0F;
}


static void sort_water_texture() {
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

static void rotate_up(RwRGBA *out, RwRGBA *in) {
    memcpy(&out[0], &in[14], sizeof(RwRGBA));
    memcpy(&out[1], &in[0], 14*sizeof(RwRGBA));
    memcpy(in, out, 15*sizeof(RwRGBA));
}

static void rotate_down(RwRGBA *out, RwRGBA *in) {
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

static void rotate_clut() {
    RwRGBA out_pallete[16];
    RwRGBA *in_pallete = (RwRGBA *)RwRasterLockPalette(texWaterclear256->raster, rwRASTERLOCKWRITE);

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

static void ReturnShadowCrossProductOffset() {
    float test_cpoffset = -0.5f;
    asm volatile ("mtc1 %0, $f1" : : "r" (*(uint32_t*)&test_cpoffset));
}

static void ReturnShadowCrossProductOffset0() {
    float test_cpoffset = -0.5f;
    asm volatile ("mtc1 %0, $f0" : : "r" (*(uint32_t*)&test_cpoffset));
}

typedef enum 
{
    PLTYPE_POINTLIGHT = 0,
    PLTYPE_SPOTLIGHT = 1,
    PLTYPE_DARKLIGHT = 2,
} ePointLightType;

typedef struct {
	CVector m_vecPosn;
	CVector m_vecDirection;
    float m_fRange;
    float m_fColorRed;
    float m_fColorGreen;
    float m_fColorBlue;
    CEntity *m_pEntityToLight;
    unsigned char m_nType; // see ePointLightType
    unsigned char m_nFogType;
    bool m_bGenerateShadows;
    char _pad0;
} CPointLight;

extern unsigned int NumLights;

#define MAX_POINTLIGHTS 32

// lights array. Count: MAX_POINTLIGHTS (32)
extern CPointLight aLights[MAX_POINTLIGHTS];

static float VectorMagnitude(CVector *vector) { return sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z); }

static unsigned short CalculateShadowStrength(float currDist, float maxDist, unsigned short maxStrength) {
    //assert(maxDist >= currDist); // Otherwise integer underflow will occur

    const float halfMaxDist = maxDist / 2.f;
    if (currDist >= halfMaxDist) { // Anything further than half the distance is faded out
        return (unsigned short)((1.f - (currDist - halfMaxDist) / halfMaxDist) * maxStrength);
    } else { // Anything closer than half the max distance is full strength
        return (unsigned short)maxStrength;
    }
}

static CVector normalize(CVector v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    CVector normalized = { v.x / length, v.y / length, v.z / length };
    return normalized;
}

static float fmaxf(float a, float b) {
    return (a > b) ? a : b;
}

static float fminf(float a, float b) {
    return (a < b) ? a : b;
}

static float ath_acosf(float x) {
  float y = sqrtf(1.0f - x * x);
  float t = atan2f(y, x);
  return t;
}

static float calculateAngle(CVector *v1, CVector *v2) {
    float dotProduct = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
    float length1 = sqrtf(v1->x * v1->x + v1->y * v1->y + v1->z * v1->z);
    float length2 = sqrtf(v2->x * v2->x + v2->y * v2->y + v2->z * v2->z);
    return ath_acosf(dotProduct / (length1 * length2));
}

static void calculateShadow(CVector *shadowPos, CVector *lightPos, CVector *lightDir, float *topX, float *topY, float *rightX, float *rightY) {
    CVector dir, norm_pos;
    if (lightDir) {
        dir = *lightDir;
    } else {
        dir.x = shadowPos->x - lightPos->x;
        dir.y = shadowPos->y - lightPos->y;
        dir.z = shadowPos->z - lightPos->z;
        
    }

    dir = normalize(dir);

    *topX = -dir.x;
    *topY = -dir.y;
    *rightX =  dir.y * 0.75f;  
    *rightY = -dir.x * 0.75f; 

    CVector defaultDir = {0.0f, 0.0f, -1.0f}; 
    float angle = calculateAngle(&defaultDir, &dir);

    float cosAngle = cosf(angle);
    float sinAngle = sinf(angle);
    
    float rotatedTopX = *topX * cosAngle - *topY * sinAngle;
    float rotatedTopY = *topX * sinAngle + *topY * cosAngle;
    float rotatedRightX = *rightX * cosAngle - *rightY * sinAngle;
    float rotatedRightY = *rightX * sinAngle + *rightY * cosAngle;

    *topX = fmaxf(fminf(rotatedTopX, 1.0f), -1.0f);
    *topY = fmaxf(fminf(rotatedTopY, 1.0f), -1.0f);
    *rightX = fmaxf(fminf(rotatedRightX, 1.0f), -1.0f);
    *rightY = fmaxf(fminf(rotatedRightY, 1.0f), -1.0f);
}

static void CShadows_CalcPedShadowValuesImpl(
    CVector *shadowPos, 
    CVector *lightPos, 
    CVector *lightDir,
    float* shadowFrontX,
    float* shadowFrontY,
    float* shadowSideX,
    float* shadowSideY,
    float* shadowDisplacementX,
    float* shadowDisplacementY
) {
    float negSunDirX;         
    float negSunDirY;         
    float magnitudeFront;     
    float invMagnitudeFront;  
    float scaleFactor;        

    CVector dir;

    if (false/*lightDir*/) {
        dir = *lightDir;
    } else {
        dir.x = lightPos->x - shadowPos->x;
        dir.y = lightPos->y - shadowPos->y;
        dir.z = lightPos->z - shadowPos->z;

        dir = normalize(dir);
    }

    negSunDirX = -dir.x;
    *shadowFrontX = negSunDirX;

    negSunDirY = -dir.y;
    *shadowFrontY = negSunDirY;

    magnitudeFront = sqrtf(*shadowFrontX * *shadowFrontX + negSunDirY * negSunDirY);
    invMagnitudeFront = 1.0f / magnitudeFront;

    scaleFactor = (magnitudeFront + 1.0f) * invMagnitudeFront;
    *shadowFrontX = scaleFactor * *shadowFrontX;
    *shadowFrontY = scaleFactor * *shadowFrontY;

    *shadowSideX = -(dir.y * invMagnitudeFront);
    *shadowSideY = dir.x * invMagnitudeFront;

    *shadowDisplacementX = negSunDirX;
    *shadowDisplacementY = negSunDirY;

    *shadowFrontX *= 0.5f;
    *shadowFrontY *= 0.5f;
    *shadowSideX *= 0.5f;
    *shadowSideY *= 0.5f;
    *shadowDisplacementX *= 0.5f;
    *shadowDisplacementY *= 0.5f;
}

typedef struct {
    float range;
    float dist;
} RangeData;

void CShadows_CalcPedShadowValues(CVector *sunPosn, float* displacementX, float* displacementY, float* frontX, float* frontY, float* sideX, float* sideY);

static bool CShadows_RenderExtraPlayerShadows(RangeData* ret) {
    bool has_lights = false;
    float min_dist = 0.0f;
    CColourSet* CTimeCycle_m_CurrentColours = (CColourSet*)0x7067C0;

    if (!CTimeCycle_m_CurrentColours->m_nShadowStrength) {
        return;
    }

    const CVector plyrPos;
    FindPlayerCoors(&plyrPos, -1);

    int i = 0;
    for (i = 0; i < NumLights; i++) {
        if (aLights[i].m_nType != PLTYPE_POINTLIGHT && aLights[i].m_nType != PLTYPE_SPOTLIGHT) {
            continue;
        }
        if (!aLights[i].m_bGenerateShadows) {
            continue;
        }
        if (aLights[i].m_fColorRed == 0.f && aLights[i].m_fColorGreen == 0.f && aLights[i].m_fColorBlue == 0.f) { // If it's black => ignore
            continue;
        }

        CVector lightToPlyr;
        lightToPlyr.x = aLights[i].m_vecPosn.x - plyrPos.x;
        lightToPlyr.y = aLights[i].m_vecPosn.y - plyrPos.y;
        lightToPlyr.z = aLights[i].m_vecPosn.z - plyrPos.z;

        float lightToPlyrDist = VectorMagnitude(&lightToPlyr);

        if (aLights[i].m_pEntityToLight) {
            if (aLights[i].m_pEntityToLight->m_nType == ENTITY_TYPE_VEHICLE) {
			    CVector posn;
                getVehicleDummyPos(&posn, *(uint16_t*)(((uint32_t)aLights[i].m_pEntityToLight) + 0x22), DUMMY_LIGHT_FRONT_MAIN);
			    if (posn.x == 0.0f) posn.x = 0.15f;
 
			    float light_offset = posn.x;

			    posn.x = 0.0f;

			    CVector* veh_pos = getVehCoordinates(aLights[i].m_pEntityToLight);
			    float angle = getVehicleHeading(aLights[i].m_pEntityToLight);
			    CVector shadow_draw;

    		    shadow_draw.x = (posn.x) * cosf(angle) - (posn.y-1.1f) * sinf(angle);
    		    shadow_draw.y = (posn.y-1.1f) * cosf(angle) + (posn.x) * sinf(angle);
			    shadow_draw.z = posn.z;

			    shadow_draw.x += veh_pos->x;
			    shadow_draw.y += veh_pos->y;
			    shadow_draw.z += veh_pos->z;

                CVector carToPlyr;
                carToPlyr.x = shadow_draw.x - plyrPos.x;
                carToPlyr.y = shadow_draw.y - plyrPos.y;
                carToPlyr.z = shadow_draw.z - plyrPos.z;

                lightToPlyrDist = VectorMagnitude(&carToPlyr);
            }
        }

        if (lightToPlyrDist >= aLights[i].m_fRange || lightToPlyrDist == 0.f) { // NOTSA: Zero check to prevent (possible) division-by-zero
            continue;
        }

        has_lights = true;

        if (fminf(lightToPlyrDist, min_dist) != min_dist) {
            ret->dist = lightToPlyrDist;
            ret->range = aLights[i].m_fRange;
            min_dist = lightToPlyrDist;
        }

        float frontX;
        float frontY;
        float sideX;
        float sideY;
        float dispX;
        float dispY;

        CVector light_pos = aLights[i].m_vecPosn;

        CVector *light_dir = (aLights[i].m_nType == PLTYPE_SPOTLIGHT) ? &aLights[i].m_vecDirection : NULL;

        CVector shadow_pos;

        //calculateShadow(&plyrPos, &light_pos, light_dir, &frontX, &frontY, &sideX, &sideY);
        CShadows_CalcPedShadowValuesImpl(&plyrPos, &light_pos, light_dir, &frontX, &frontY, &sideX, &sideY, &dispX, &dispY);

        shadow_pos.x = plyrPos.x + dispX;
        shadow_pos.y = plyrPos.y + dispY;

        shadow_pos.z = plyrPos.z;

        CShadows_StoreShadowToBeRenderedSingle(
            1,
            *(RwTexture**)0x66B204,
            &shadow_pos,
            frontX,
            frontY,
            sideX, 
            sideY,
            CalculateShadowStrength(
                lightToPlyrDist,
                aLights[i].m_fRange,
                CTimeCycle_m_CurrentColours->m_nShadowStrength
            ),
            0, 0, 0,
            4.0f,
            false,
            1.f,
            NULL,
            true
        );
    }

    return has_lights;
}

static void CShadows_StoreShadowForPlayer(uint8_t type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, uint8_t red, uint8_t green, uint8_t blue, float zDistance, bool drawOnWater, float scale, void* realTimeShadow, bool drawOnBuildings) {
    short ped_intensity = intensity;
    RangeData distance;

    if (drawOnBuildings) {
        if (CShadows_RenderExtraPlayerShadows(&distance)) {
            if (distance.dist > 2.f) {
                ped_intensity = (short)((float)intensity * ((float)distance.dist / distance.range));
            } else {
                ped_intensity = 32;
            }
        }

    }

    CShadows_StoreShadowToBeRenderedSingle(type, texture, posn, topX, topY, rightX, rightY, ped_intensity, red, green, blue, zDistance, drawOnWater, scale, realTimeShadow, drawOnBuildings);
}



void installSkyEdgeEngine()
{  
    InjectRubbishPatches();
    //injectRainGraphicsPatches();
    InjectSkyEdgePostEffects();

    //MakeNop(0x1190E4);

    //RedirectCall(0x119358, ReturnShadowCrossProductOffset);
    //MakeNop(0x11935C);

    //RedirectCall(0x1198F8, ReturnShadowCrossProductOffset0);
    //RedirectCall(0x11A0C0, ReturnShadowCrossProductOffset0);

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

    //RedirectCall(0x26F930, CShadows_RenderExtraPlayerShadows);
    RedirectCall(0x1156F0, CShadows_StoreShadowForPlayer);
    WriteDword(0x17E018, 0x0080402D);
    WriteDword(0x17E064, 0x0080402D);
    WriteDword(0x17E168, 0x0080402D);

    WriteDword(0x17E01C, 0x0260482D);
    WriteDword(0x17E068, 0x0260482D);

    //RedirectCall(0x24693C, SkyEdgeCameraClear);
}
