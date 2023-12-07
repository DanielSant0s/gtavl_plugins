#include <injector.h>
#include <rwcore.h>
#include <secore.h>
#include <CRGBA.h>
#include <CVector.h>
#include <CVehicle.h>
#include <CTexture.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <CCam.h>
#include <math.h>

#include "modules.h"

void (*CGame_Init2)(const char*);

void* Camera = (void*)0x66C2FC;

void (*hook_RenderEffects)();

int (*CCustomCarEnvMapPipeline__SetFxEnvTexture)(void *a1, RwTexture *a2) = (int (*)(void *a1, RwTexture *a2))0x507B90;
int (*CCustomCarEnvMapPipeline__SetFxSpecTexture)(void *a1, RwTexture *a2) = (int (*)(void *a1, RwTexture *a2))0x507D00;

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

int _start()
{  
    InjectRubbishPatches();

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

    return 0;
}
