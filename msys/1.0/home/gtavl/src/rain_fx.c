#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CTexture.h>
#include "renderbuffer.h"
#include <common.h>
#include <math.h>

#include <CPad.h>

#include "bios_alloc.h"
/*
CVector test_coords[] = {
    { .x=229.837296f, .y=-1729.875366f, .z=4.147039f },
    { .x=221.355927f, .y=-1734.910278f, .z=4.265414f },
    { .x=214.969986f, .y=-1741.270386f, .z=4.539824f },
    { .x=223.036880f, .y=-1744.643433f, .z=4.665923f },
    { .x=233.530350f, .y=-1739.483032f, .z=4.276421f },
    { .x=246.644180f, .y=-1733.052612f, .z=3.939794f },
    { .x=256.786865f, .y=-1735.581177f, .z=3.943499f },
    { .x=262.942139f, .y=-1729.911865f, .z=3.937498f },
    { .x=267.957397f, .y=-1724.466431f, .z=4.346838f },
    { .x=263.271698f, .y=-1718.455200f, .z=5.730078f },
    { .x=262.740387f, .y=-1711.545410f, .z=7.443019f },
    { .x=273.783722f, .y=-1710.554688f, .z=7.817119f },
    { .x=282.287720f, .y=-1717.366943f, .z=6.862543f },
    { .x=285.039459f, .y=-1738.829102f, .z=4.089005f },
    { .x=273.846771f, .y=-1740.748901f, .z=4.187046f },
    { .x=0.0f, .y=0.0f, .z=0.0f },
    { .x=0.0f, .y=0.0f, .z=0.0f },
    { .x=0.0f, .y=0.0f, .z=0.0f },
    { .x=0.0f, .y=0.0f, .z=0.0f },
    { .x=0.0f, .y=0.0f, .z=0.0f },
};
*/
static void (*CGame_ShutdownRenderWare)();
static void (*CMovingThings_Render)();
static void (*RenderEffects)();
static void (*CGame_Init1)(const char*);

#define pCam 0x66C2FC

static RwRaster *pSkyRaster;
static RwTexture *ScreenSpaceSkyTexture;

float *CWeather_Rain = (float*)0x66BD5C;

typedef struct {
    CVector pos;

    uint32_t seed[5];

    uint32_t seed_timer;

    uint16_t selected_seed;

    uint16_t num_triangles;

    float cur_radius;
    float radius; 

    float variance;
    
    RwRGBA cur_color;
    RwRGBA color;
} CWaterPuddle;

#define MAX_PUDDLES 20

CWaterPuddle *puddles;

static void CWaterPuddles_Init(const char* a1) {
    CGame_Init1(a1);
    /*

    puddles = br_alloc_array(CWaterPuddle, MAX_PUDDLES);

    if (puddles) {
        int i; 
        for (i = 0; i < MAX_PUDDLES; i++) {
            puddles[i].pos = test_coords[i];
            
            puddles[i].seed[0] = rand();
            puddles[i].seed[1] = rand();
            puddles[i].seed[2] = rand();
            puddles[i].seed[3] = rand();
            puddles[i].seed[4] = rand();

            puddles[i].seed_timer = 0;
            puddles[i].selected_seed = 0;

            puddles[i].num_triangles = randint(10, 20);

            puddles[i].radius = random_float(1.2f, 3.5f);

            puddles[i].cur_radius = puddles[i].radius;

            puddles[i].variance = 0.3f;

            puddles[i].color.r = 64;
            puddles[i].color.g = 64;
            puddles[i].color.b = 64;
            puddles[i].color.a = 64;

            puddles[i].cur_color = puddles[i].color;


        }
    }

    */

    /*

    pSkyRaster = RwRasterCreate(64, 64, 24, rwRASTERTYPETEXTURE | rwRASTERDONTALLOCATE);

    RpSkyTexCacheRasterLock(pSkyRaster, true);
    
    
    ScreenSpaceSkyTexture = RwTextureCreate(pSkyRaster);
    ScreenSpaceSkyTexture->filterAddressing = rwFILTERLINEAR;

    */

    //RwRaster* camRaster = *(RwRaster**)(pCam + 96);
    //RwRaster* camZRaster = *(RwRaster**)(pCam + 100);
}

static void CWaterPuddles_Update() {
    /*if (CWeather_Rain > 0.02f) {
        seGetBackBufferOffset(pSkyRaster, 0, 0);
    }*/

    RenderEffects();   
}

static void CWaterPuddles_Render() {
    CMovingThings_Render();

    /*if (CGame_currArea > 0 || !puddles || *CWeather_Rain <= 0.02f)
        return;

    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)false);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)true);

    RwRenderStateSet(rwRENDERSTATESRCBLEND, rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, rwBLENDINVDESTCOLOR);

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, ScreenSpaceSkyTexture->raster);

    int i; 
    for (i = 0; i < MAX_PUDDLES; i++) {
        TempBufferIndicesStored = 0;
        TempBufferVerticesStored = 0;

        int v = TempBufferVerticesStored;
        int index = TempBufferIndicesStored;

        int seed = puddles[i].seed[0];

        unsigned int resetSeed = rand() / 2;

        srand(seed);

        int j;
        for (j = 0; j <= puddles[i].num_triangles; j++) {
            float angle =  (float)j / puddles[i].num_triangles * 2.0f * M_PI;
            //float nextAngle = (float)(j + 1) / puddles[i].num_triangles * 2.0f * M_PI;

            float offsetX = puddles[i].cur_radius * random_float(1.0f-puddles[i].variance, 1.0f+puddles[i].variance) * cosf(angle);
            float offsetY = puddles[i].cur_radius * random_float(1.0f-puddles[i].variance, 1.0f+puddles[i].variance) * sinf(angle);

            TempBufferRenderVertices[v + j].pos.x = puddles[i].pos.x + offsetX;
            TempBufferRenderVertices[v + j].pos.y = puddles[i].pos.y + offsetY;
            TempBufferRenderVertices[v + j].pos.z = CWorld_FindGroundZFor3DCoord(puddles[i].pos.x + offsetX, puddles[i].pos.y + offsetY, puddles[i].pos.z, NULL, NULL) + 0.05f;
            TempBufferRenderVertices[v + j].color = puddles[i].color;
            TempBufferRenderVertices[v + j].u = (cosf(angle) + 1.0f) * random_float(0.2f, 0.7f);
            TempBufferRenderVertices[v + j].v = (sinf(angle) + 1.0f) * random_float(0.2f, 0.7f);

            if (j > 1) {
                TempBufferRenderIndexList[index++] = TempBufferVerticesStored;
                TempBufferRenderIndexList[index++] = v + j - 1;
                TempBufferRenderIndexList[index++] = v + j;
            }

            
        }


        TempBufferVerticesStored += puddles[i].num_triangles + 1;
        TempBufferIndicesStored = index;

        if (TempBufferIndicesStored != 0) {
            if (RwIm3DTransform(TempBufferRenderVertices, TempBufferVerticesStored, NULL, 1)) {
                RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRISTRIP, TempBufferRenderIndexList, TempBufferIndicesStored);
                RwIm3DEnd();
            }
        }
    }

    RwRenderStateSet(rwRENDERSTATESRCBLEND, rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, rwBLENDINVSRCALPHA);

    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)false);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)false);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
    */
}

static void CWaterPuddles_Shutdown() {
    CGame_ShutdownRenderWare();
}

void injectRainGraphicsPatches() {
    CGame_Init1 =              (void (*)(const char*))ReadCall(0x242B9C);
    RenderEffects =            (void (*)())ReadCall(0x246990);
    CMovingThings_Render =     (void (*)())ReadCall(0x246EA8);
    CGame_ShutdownRenderWare = (void (*)())ReadCall(0x247388);

    RedirectCall(0x242B9C, &CWaterPuddles_Init);
    RedirectCall(0x246990, &CWaterPuddles_Update);
    RedirectCall(0x246EA8, &CWaterPuddles_Render);
    RedirectCall(0x247388, &CWaterPuddles_Shutdown);
}
